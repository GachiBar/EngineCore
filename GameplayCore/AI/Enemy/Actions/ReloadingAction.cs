using GameplayCore.Components;
using GameplayCore.Mathematics;
using System;
using System.Collections.Generic;

namespace GameplayCore.AI.Enemy.Actions
{
    public class ReloadingAction : AIAction
    {
        public override bool CheckPreconditions(GameObject gameObject, AIState state)
        {
            return state.GetBoolValue("NeedReloading");
        }

        public override IEnumerable<AIExecutionState> Execute(GameObject gameObject, AIState state)
        {
            var reloadingTime = state.GetFloatValue("ReloadingTime");
            var aim = state.GetGameObjectValue("Aim");

            var enemyTransform = gameObject.GetComponent<TransformComponent>();
            var enemyController = gameObject.GetComponent<RangeEnemyControllerComponent>();
            var aimTransform = aim?.GetComponent<TransformComponent>();

            for (float timer = 0; timer < reloadingTime; timer += Time.FixedDeltaTime)
            {
                if (aimTransform != null)
                {
                    var axis = Vector3.UnitX + Vector3.UnitZ;
                    var direction = (aimTransform.Position - enemyTransform.Position) * axis;
                    direction.Normalize();
                    enemyController.LoockInDirection(direction);
                }

                enemyController.Reload(timer, reloadingTime);
                yield return AIExecutionState.InProgress;
            }

            enemyController.Reload(reloadingTime, reloadingTime);
            state.SetBoolValue("NeedReloading", false);
            yield return AIExecutionState.Finished;
        }

        public override void PlanEffects(GameObject gameObject, AIState state)
        {
            state.SetBoolValue("NeedReloading", false);
        }
    }
}

using GameplayCore.Components;
using GameplayCore.Mathematics;
using System;
using System.Collections.Generic;

namespace GameplayCore.AI.Enemy.Actions
{
    public class ReloadingAction : Action
    {
        public override bool CheckPreconditions(GameObject gameObject, State state)
        {
            return state.GetBoolValue("NeedReloading");
        }

        public override IEnumerable<ExecutionState> Execute(GameObject gameObject, State state)
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

                // TODO: some visual effect
                Console.WriteLine("Still reloading...");
                yield return ExecutionState.InProgress;
            }

            Console.WriteLine("Reloading completed!");
            state.SetBoolValue("NeedReloading", false);
            yield return ExecutionState.Finished;
        }

        public override void PlanEffects(GameObject gameObject, State state)
        {
            state.SetBoolValue("NeedReloading", false);
        }
    }
}

using GameplayCore.Components;
using GameplayCore.Mathematics;
using System.Collections.Generic;

namespace GameplayCore.AI.Enemy.Actions
{
    public class FleeAction : Action
    {
        public override bool CheckPreconditions(GameObject gameObject, State state)
        {
            return state.GetBoolValue("HasAim") == true;
        }

        public override IEnumerable<ExecutionState> Execute(GameObject gameObject, State state)
        {
            var enemyHealth = gameObject.GetComponent<HealthComponent>();
            var enemyTransform = gameObject.GetComponent<TransformComponent>();
            var enemyController = gameObject.GetComponent<RangeEnemyControllerComponent>();

            var aim = state.GetGameObjectValue("Aim");
            var aimTransform = aim.GetComponent<TransformComponent>();

            while (enemyHealth.Health / enemyHealth.MaxHealth < 0.5f)
            {
                var viewDirection = GetViewDirection(aimTransform.Position, enemyTransform.Position);
                enemyController.MoveInDirection(viewDirection);
                enemyController.LoockInDirection(viewDirection);
                yield return ExecutionState.InProgress;
            }

            state.SetBoolValue("NeedFlee", false);
            yield return ExecutionState.Finished;
        }

        public override void PlanEffects(GameObject gameObject, State state)
        {
            state.SetBoolValue("NeedFlee", false);
        }

        private Vector3 GetViewDirection(Vector3 from, Vector3 to)
        {
            var axis = Vector3.UnitX + Vector3.UnitZ;
            var direction = to - from;
            var viewDirection = direction * axis;
            viewDirection.Normalize();
            return viewDirection;
        }
    }
}

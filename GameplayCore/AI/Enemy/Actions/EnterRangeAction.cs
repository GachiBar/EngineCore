using GameplayCore.Components;
using GameplayCore.Mathematics;
using System.Collections.Generic;

namespace GameplayCore.AI.Enemy.Actions
{
    public class EnterRangeAction : Action
    {
        public override bool CheckPreconditions(GameObject gameObject, State state)
        {
            return state.GetBoolValue("HasAim") == true;
        }

        public override IEnumerable<ExecutionState> Execute(GameObject gameObject, State state)
        {
            var aim = state.GetGameObjectValue("Aim");
            var enemyTransform = gameObject.GetComponent<TransformComponent>();
            var enemyController = gameObject.GetComponent<RangeEnemyControllerComponent>();
            var aimTransform = aim.GetComponent<TransformComponent>();

            while ((enemyTransform.Position - aimTransform.Position).Length() > enemyController.Range)
            {                
                var axis = Vector3.UnitX + Vector3.UnitZ;
                var direction = (aimTransform.Position - enemyTransform.Position) * axis;
                direction.Normalize();
                enemyController.MoveInDirection(direction);
                yield return ExecutionState.InProgress;
            }

            state.SetBoolValue("CanShoot", true);
            yield return ExecutionState.Finished;
        }

        public override void PlanEffects(GameObject gameObject, State state)
        {
            state.SetBoolValue("CanShoot", true);
        }
    }
}

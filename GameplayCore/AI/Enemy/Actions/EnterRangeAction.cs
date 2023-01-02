using GameplayCore.Components;
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
            var range = state.GetFloatValue("Range");
            var speed = state.GetFloatValue("Speed");

            var enemyTransform = gameObject.GetComponent<TransformComponent>();
            var enemyRigidbody = gameObject.GetComponent<RigidbodyComponent>();
            var aimTransform = aim.GetComponent<TransformComponent>();

            while ((enemyTransform.Position - aimTransform.Position).Length() > range)
            {
                var direction = aimTransform.Position - enemyTransform.Position;
                direction.Normalize();

                enemyRigidbody.Velocity = direction * speed;

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

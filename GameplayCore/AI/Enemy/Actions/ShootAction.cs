using GameplayCore.Components;
using GameplayCore.Mathematics;
using System.Collections.Generic;

namespace GameplayCore.AI.Enemy.Actions
{
    public class ShootAction : AIAction
    {
        public override bool CheckPreconditions(GameObject gameObject, AIState state)
        {
            return state.GetBoolValue("CanShoot") == true;
        }

        public override IEnumerable<AIExecutionState> Execute(GameObject gameObject, AIState state)
        {
            var aim = state.GetGameObjectValue("Aim");
            var rigidbody = gameObject.GetComponent<RigidbodyComponent>();
            var rangeController = gameObject.GetComponent<RangeEnemyControllerComponent>();
            rigidbody.Velocity = Vector3.Zero;
            rangeController.Shoot(aim);

            state.SetBoolValue("CanShoot", false);
            state.SetBoolValue("NeedReloading", true);
            yield return AIExecutionState.Finished;
        }

        public override void PlanEffects(GameObject gameObject, AIState state)
        {
            state.SetBoolValue("CanShoot", false);
            state.SetBoolValue("NeedReloading", true);
        }
    }
}

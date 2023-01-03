using GameplayCore.Components;
using GameplayCore.Mathematics;
using System.Collections.Generic;

namespace GameplayCore.AI.Enemy.Actions
{
    public class ShootAction : Action
    {
        public override bool CheckPreconditions(GameObject gameObject, State state)
        {
            return state.GetBoolValue("CanShoot") == true;
        }

        public override IEnumerable<ExecutionState> Execute(GameObject gameObject, State state)
        {
            var aim = state.GetGameObjectValue("Aim");
            var rigidbody = gameObject.GetComponent<RigidbodyComponent>();
            var rangeController = gameObject.GetComponent<RangeEnemyControllerComponent>();
            rigidbody.Velocity = Vector3.Zero;
            rangeController.Shoot(aim);

            state.SetBoolValue("HasAim", false);
            state.SetBoolValue("CanShoot", false);
            state.SetBoolValue("NeedReloading", true);
            yield return ExecutionState.Finished;
        }

        public override void PlanEffects(GameObject gameObject, State state)
        {
            state.SetBoolValue("HasAim", false);
            state.SetBoolValue("CanShoot", false);
            state.SetBoolValue("NeedReloading", true);
        }
    }
}

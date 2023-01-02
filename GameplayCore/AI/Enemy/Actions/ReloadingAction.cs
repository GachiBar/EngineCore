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

            for (float timer = 0; timer < reloadingTime; timer += Time.FixedDeltaTime)
            {
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

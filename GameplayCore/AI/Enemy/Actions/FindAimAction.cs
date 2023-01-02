using GameplayCore.Components;
using System.Collections.Generic;
using System.Linq;

namespace GameplayCore.AI.Enemy.Actions
{
    public class FindAimAction : Action
    {
        public override bool CheckPreconditions(GameObject gameObject, State state)
        {
            return true;
        }

        public override IEnumerable<ExecutionState> Execute(GameObject gameObject, State state)
        {
            var aim = gameObject.Scene.FirstOrDefault(go => go.GetComponent<FpsControllerComponent>() != null);

            if (aim == null)
            {
                yield return ExecutionState.Interrupted;
            }

            state.SetGameObjectValue("Aim", aim);
            state.SetBoolValue("HasAim", true);
            yield return ExecutionState.Finished;
        }

        public override void PlanEffects(GameObject gameObject, State state)
        {
            state.SetBoolValue("HasAim", true);
        }
    }
}

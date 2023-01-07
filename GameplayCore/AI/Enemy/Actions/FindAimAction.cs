using GameplayCore.Components;
using System.Collections.Generic;
using System.Linq;

namespace GameplayCore.AI.Enemy.Actions
{
    public class FindAimAction : AIAction
    {
        public override bool CheckPreconditions(GameObject gameObject, AIState state)
        {
            return true;
        }

        public override IEnumerable<AIExecutionState> Execute(GameObject gameObject, AIState state)
        {
            var aim = gameObject.Scene.FirstOrDefault(go => go.GetComponent<FpsControllerComponent>() != null);

            if (aim == null)
            {
                yield return AIExecutionState.Interrupted;
            }

            state.SetGameObjectValue("Aim", aim);
            state.SetBoolValue("HasAim", true);
            yield return AIExecutionState.Finished;
        }

        public override void PlanEffects(GameObject gameObject, AIState state)
        {
            state.SetBoolValue("HasAim", true);
        }
    }
}

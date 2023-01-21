using GameplayCore.Components;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameplayCore.AI.CaravanAI.Actions
{
    public class GuardAttackAction : AIAction
    {
        public override bool CheckPreconditions(GameObject gameObject, AIState state)
        {
            return state.GetBoolValue("isNearTarget") && state.GetBoolValue("hasTarget");
        }

        public override IEnumerable<AIExecutionState> Execute(GameObject gameObject, AIState state)
        {
            var selfController = gameObject.GetComponent<GuardComponent>();
            var target = selfController.TargetGO;
            RobbersUnitController targetController = null;
            if (target != null)
                 targetController = target.GetComponent<RobbersUnitController>();
            else 
                yield return AIExecutionState.Interrupted;
            if(targetController != null)
                targetController.takeDamage(1);
            for (float timer = 0; timer < 2.15f; timer += Time.FixedDeltaTime)
            {
                yield return AIExecutionState.InProgress;
            }
            
            state.SetBoolValue("isNearTarget", false);
            state.SetBoolValue("isNearBaseTile", false);
            state.SetBoolValue("hasTarget", false);
            state.SetBoolValue("isIdle", true);
            yield return AIExecutionState.Finished;
        }

        public override void PlanEffects(GameObject gameObject, AIState state)
        {
            state.SetBoolValue("isNearTarget", false);
            state.SetBoolValue("isNearBaseTile", false);
            state.SetBoolValue("hasTarget", false);
            state.SetBoolValue("isIdle", true);
        }
    }
}

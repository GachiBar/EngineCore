using GameplayCore.Components;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameplayCore.AI.CaravanAI.Actions
{
    public class CollectGoldAction : AIAction
    {
        public override bool CheckPreconditions(GameObject gameObject, AIState state)
        {
            return state.GetBoolValue("isNearCaravan") && !state.GetBoolValue("collectedGold");
        }

        public override IEnumerable<AIExecutionState> Execute(GameObject gameObject, AIState state)
        {

            //var enemyTransform = gameObject.GetComponent<TransformComponent>();

            for (float timer = 0; timer < 1.5; timer += Time.FixedDeltaTime)
            {
                Console.WriteLine("Get Gold in progress: {0:0.00}", timer/1.5 * 100);

                yield return AIExecutionState.InProgress;
            }
            state.SetBoolValue("collectedGold", true);
            yield return AIExecutionState.Finished;
        }

        public override void PlanEffects(GameObject gameObject, AIState state)
        {
            state.SetBoolValue("collectedGold", true);
        }
    }
}

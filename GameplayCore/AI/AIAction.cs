using System.Collections.Generic;

namespace GameplayCore.AI
{

    public abstract class AIAction
    {
        public abstract bool CheckPreconditions(GameObject gameObject, AIState state);
        public abstract void PlanEffects(GameObject gameObject, AIState state);
        public abstract IEnumerable<AIExecutionState> Execute(GameObject gameObject, AIState state);
    }
}

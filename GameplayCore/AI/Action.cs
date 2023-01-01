using System.Collections.Generic;

namespace GameplayCore.AI
{

    public abstract class Action
    {
        public abstract bool CheckPreconditions(GameObject gameObject, State state);
        public abstract void PlanEffects(GameObject gameObject, State state);
        public abstract IEnumerable<ExecutionState> Execute(GameObject gameObject, State state);
    }
}

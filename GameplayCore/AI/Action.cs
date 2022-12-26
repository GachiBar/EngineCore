using System.Collections.Generic;

namespace GameplayCore.AI
{

    public abstract class Action
    {
        public abstract bool CheckPreconditions(State state);
        public abstract void PlanEffects(State state);
        public abstract IEnumerable<ExecutionState> Execute(State state);
    }
}

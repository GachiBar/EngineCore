using System;
using System.Collections.Generic;

namespace GameplayCore.AI
{
    public class Plan
    {
        private List<Action> _actions;

        public ExecutionState ExecutionState { get; private set; }

        public Plan(IEnumerable<Action> actions)
        {
            _actions = new List<Action>(actions);
        }

        public IEnumerable<ExecutionState> Execute(State state)
        {
            bool isInterrupted = false;
            ExecutionState = ExecutionState.InProgress;

            foreach (var action in _actions)
            {
                foreach (var executionState in action.Execute(state))
                {
                    switch (executionState)
                    {
                        case ExecutionState.InProgress:
                            yield return ExecutionState.InProgress;
                            break;
                        case ExecutionState.Interrupted:
                            isInterrupted = true;
                            break;
                        case ExecutionState.Finished:
                            yield return ExecutionState.InProgress;
                            break;
                        default:
                            throw new ArgumentOutOfRangeException(nameof(executionState));
                    }
                }

                if (isInterrupted)
                {
                    break;
                }
            }

            if (isInterrupted)
            {
                ExecutionState = ExecutionState.Interrupted;
                yield return ExecutionState.Interrupted;
            }
            else
            {
                ExecutionState = ExecutionState.Finished;
                yield return ExecutionState.Finished;
            }            
        }
    }
}

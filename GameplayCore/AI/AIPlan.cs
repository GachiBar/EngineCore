using System;
using System.Collections.Generic;

namespace GameplayCore.AI
{
    public class AIPlan
    {
        private List<AIAction> _actions;

        public AIExecutionState ExecutionState { get; private set; }

        public AIPlan(IEnumerable<AIAction> actions)
        {
            _actions = new List<AIAction>(actions);
            ExecutionState = AIExecutionState.InProgress;
        }

        public IEnumerable<AIExecutionState> Execute(GameObject gameObject, AIState state)
        {
            bool isInterrupted = false;
            yield return AIExecutionState.InProgress;

            foreach (var action in _actions)
            {
                foreach (var executionState in action.Execute(gameObject, state))
                {
                    switch (executionState)
                    {
                        case AIExecutionState.InProgress:
                            yield return AIExecutionState.InProgress;
                            break;
                        case AIExecutionState.Interrupted:
                            isInterrupted = true;
                            break;
                        case AIExecutionState.Finished:
                            yield return AIExecutionState.InProgress;
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
                ExecutionState = AIExecutionState.Interrupted;
                yield return AIExecutionState.Interrupted;
            }
            else
            {
                ExecutionState = AIExecutionState.Finished;
                yield return AIExecutionState.Finished;
            }            
        }
    }
}

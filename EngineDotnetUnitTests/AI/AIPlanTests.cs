using GameplayCore;
using GameplayCore.AI;
using NUnit.Framework;
using System.Collections.Generic;

namespace EngineDotnetUnitTests.AI
{
    public class AIPlanTests
    {
        public class DummyAction : AIAction
        {
            public readonly int ProcessingTime;

            public DummyAction(int processingTime)
            {
                ProcessingTime = processingTime;
            }

            public override bool CheckPreconditions(GameObject gameObject, AIState state)
            {
                return true;
            }

            public override IEnumerable<AIExecutionState> Execute(GameObject gameObject, AIState state)
            {
                for (int i = 0; i < ProcessingTime; i++)
                {
                    yield return AIExecutionState.InProgress;
                }

                yield return AIExecutionState.Finished;
            }

            public override void PlanEffects(GameObject gameObject, AIState state)
            {
                return;
            }
        }

        public class InterrutAction : AIAction
        {
            public override bool CheckPreconditions(GameObject gameObject, AIState state)
            {
                return true;
            }

            public override IEnumerable<AIExecutionState> Execute(GameObject gameObject, AIState state)
            {
                yield return AIExecutionState.Interrupted;
            }

            public override void PlanEffects(GameObject gameObject, AIState state)
            {
                return;
            }
        }

        [Test]
        public void Execute_UninterruptibleActions_PlanFinished()
        {
            var actions = new List<AIAction>
            {
                new DummyAction(0),
                new DummyAction(1),
                new DummyAction(2),
                new DummyAction(3),
            };

            var plan = new AIPlan(actions);

            foreach (var _ in plan.Execute(null, null));

            Assert.IsTrue(plan.ExecutionState == AIExecutionState.Finished);
        }

        [Test]
        public void Execute_InterruptedActions_PlanInterrupted()
        {
            var actions = new List<AIAction>
            {
                new DummyAction(0),
                new DummyAction(1),
                new InterrutAction(),
                new DummyAction(2),
                new DummyAction(3),
            };

            var plan = new AIPlan(actions);

            foreach (var _ in plan.Execute(null, null));

            Assert.IsTrue(plan.ExecutionState == AIExecutionState.Interrupted);
        }
    }
}

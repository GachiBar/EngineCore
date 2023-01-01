using GameplayCore;
using GameplayCore.AI;
using NUnit.Framework;
using System.Collections.Generic;

namespace EngineDotnetUnitTests.AI
{
    public class PlanTests
    {
        public class DummyAction : Action
        {
            public readonly int ProcessingTime;

            public DummyAction(int processingTime)
            {
                ProcessingTime = processingTime;
            }

            public override bool CheckPreconditions(GameObject gameObject, State state)
            {
                return true;
            }

            public override IEnumerable<ExecutionState> Execute(GameObject gameObject, State state)
            {
                for (int i = 0; i < ProcessingTime; i++)
                {
                    yield return ExecutionState.InProgress;
                }

                yield return ExecutionState.Finished;
            }

            public override void PlanEffects(GameObject gameObject, State state)
            {
                return;
            }
        }

        public class InterrutAction : Action
        {
            public override bool CheckPreconditions(GameObject gameObject, State state)
            {
                return true;
            }

            public override IEnumerable<ExecutionState> Execute(GameObject gameObject, State state)
            {
                yield return ExecutionState.Interrupted;
            }

            public override void PlanEffects(GameObject gameObject, State state)
            {
                return;
            }
        }

        [Test]
        public void Execute_UninterruptibleActions_PlanFinished()
        {
            var actions = new List<Action>
            {
                new DummyAction(0),
                new DummyAction(1),
                new DummyAction(2),
                new DummyAction(3),
            };

            var plan = new Plan(actions);

            foreach (var _ in plan.Execute(null, null));

            Assert.IsTrue(plan.ExecutionState == ExecutionState.Finished);
        }

        [Test]
        public void Execute_InterruptedActions_PlanInterrupted()
        {
            var actions = new List<Action>
            {
                new DummyAction(0),
                new DummyAction(1),
                new InterrutAction(),
                new DummyAction(2),
                new DummyAction(3),
            };

            var plan = new Plan(actions);

            foreach (var _ in plan.Execute(null, null));

            Assert.IsTrue(plan.ExecutionState == ExecutionState.Interrupted);
        }
    }
}

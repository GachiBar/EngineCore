using GameplayCore.AI;
using NUnit.Framework;
using System.Collections.Generic;

namespace EngineDotnetUnitTests.AI
{
    public class PlannerTests
    {
        public class OmeletteGoal : Goal
        {
            public override int DistanceTo(State state)
            {
                int distance = 3;

                if (state.GetBoolValue("HasOmelette"))
                {
                    return 0;
                }
                if (state.GetBoolValue("HasEggs"))
                {
                    distance -= 1;
                }
                if (state.GetBoolValue("HasMilk"))
                {
                    distance -= 1;
                }

                return distance;
            }

            public override GoalPriority GetPriority()
            {
                throw new System.NotImplementedException();
            }
        }

        public class GetEggsAction : Action
        {
            public override bool CheckPreconditions(State state)
            {
                return true;
            }

            public override IEnumerable<ExecutionState> Execute(State state)
            {
                state.SetBoolValue("HasEggs", true);
                yield return ExecutionState.Finished;
            }

            public override void PlanEffects(State state)
            {
                state.SetBoolValue("HasEggs", true);
            }
        }

        public class GetMilkAction : Action
        {
            public override bool CheckPreconditions(State state)
            {
                return true;
            }

            public override IEnumerable<ExecutionState> Execute(State state)
            {
                state.SetBoolValue("HasMilk", true);
                yield return ExecutionState.Finished;
            }

            public override void PlanEffects(State state)
            {
                state.SetBoolValue("HasMilk", true);
            }
        }

        public class FryOmeletteAction : Action
        {
            public override bool CheckPreconditions(State state)
            {
                return state.GetBoolValue("HasEggs") 
                    && state.GetBoolValue("HasMilk");
            }

            public override IEnumerable<ExecutionState> Execute(State state)
            {
                state.SetBoolValue("HasEggs", false);
                yield return ExecutionState.InProgress;

                state.SetBoolValue("HasMilk", false);
                yield return ExecutionState.InProgress;

                state.SetBoolValue("HasOmelette", true);
                yield return ExecutionState.Finished;
            }

            public override void PlanEffects(State state)
            {
                state.SetBoolValue("HasEggs", false);
                state.SetBoolValue("HasMilk", false);
                state.SetBoolValue("HasOmelette", true);
            }
        }

        [Test]
        public void PlannerTest()
        {
            var state = new State();
            state.SetBoolValue("HasEggs", false);
            state.SetBoolValue("HasMilk", false);
            state.SetBoolValue("HasOmelette", false);

            var goal = new OmeletteGoal();

            var actions = new List<Action>()
            {
                new GetEggsAction(),
                new GetMilkAction(),
                new FryOmeletteAction(),
            };

            var planner = new Planner();

            var plan = planner.MakePlan(state, goal, actions);
        }
    }
}

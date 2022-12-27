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
                int distance = 5;

                if (state.GetBoolValue("HasOmelette"))
                {
                    return 0;
                }
                if (state.GetBoolValue("HasBeatenEggs"))
                {
                    return 1;
                }
                if (state.GetBoolValue("HasEggs"))
                {
                    distance -= 1;
                }
                if (state.GetBoolValue("HasMilk"))
                {
                    distance -= 1;
                }
                if (state.GetBoolValue("HasOil"))
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

        public class GetOilAction : Action
        {
            public override bool CheckPreconditions(State state)
            {
                return true;
            }

            public override IEnumerable<ExecutionState> Execute(State state)
            {
                state.SetBoolValue("HasOil", true);
                yield return ExecutionState.Finished;
            }

            public override void PlanEffects(State state)
            {
                state.SetBoolValue("HasOil", true);
            }
        }

        public class ShakeEggsAction : Action
        {
            public override bool CheckPreconditions(State state)
            {
                return state.GetBoolValue("HasEggs")
                    && state.GetBoolValue("HasMilk");
            }

            public override IEnumerable<ExecutionState> Execute(State state)
            {
                state.SetBoolValue("HasEggs", false);
                state.SetBoolValue("HasMilk", false);
                state.SetBoolValue("HasBeatenEggs", true);
                yield return ExecutionState.Finished;
            }

            public override void PlanEffects(State state)
            {
                state.SetBoolValue("HasEggs", false);
                state.SetBoolValue("HasMilk", false);
                state.SetBoolValue("HasBeatenEggs", true);
            }
        }

        public class FryOmeletteAction : Action
        {
            public override bool CheckPreconditions(State state)
            {
                return state.GetBoolValue("HasOil")
                    && state.GetBoolValue("HasBeatenEggs");
            }

            public override IEnumerable<ExecutionState> Execute(State state)
            {
                state.SetBoolValue("HasOil", false);
                yield return ExecutionState.InProgress;

                state.SetBoolValue("HasBeatenEggs", false);
                yield return ExecutionState.InProgress;

                state.SetBoolValue("HasOmelette", true);
                yield return ExecutionState.Finished;
            }

            public override void PlanEffects(State state)
            {
                state.SetBoolValue("HasOil", false);
                state.SetBoolValue("HasBeatenEggs", false);
                state.SetBoolValue("HasOmelette", true);
            }
        }

        [Test]
        public void MakePlan_ActionsCanReachTheGoal_PlanIsMaked()
        {
            var state = new State();
            state.SetBoolValue("HasEggs", false);
            state.SetBoolValue("HasMilk", false);
            state.SetBoolValue("HasOil", false);
            state.SetBoolValue("HasBeatenEggs", false);
            state.SetBoolValue("HasOmelette", false);

            var goal = new OmeletteGoal();

            var actions = new List<Action>()
            {
                new GetEggsAction(),
                new GetMilkAction(),
                new GetOilAction(),
                new ShakeEggsAction(),
                new FryOmeletteAction(),
            };

            var planner = new Planner();

            var plan = planner.MakePlan(state, goal, actions);

            foreach (var _ in plan.Execute(state));

            Assert.IsTrue(plan.ExecutionState == ExecutionState.Finished);
            Assert.IsTrue(state.GetBoolValue("HasOmelette"));
        }

        [Test]
        public void MakePlan_ActionsCanNotReachTheGoal_PlanIsNull()
        {
            var state = new State();
            state.SetBoolValue("HasEggs", false);
            state.SetBoolValue("HasMilk", false);
            state.SetBoolValue("HasOil", false);
            state.SetBoolValue("HasBeatenEggs", false);
            state.SetBoolValue("HasOmelette", false);

            var goal = new OmeletteGoal();

            var actions = new List<Action>()
            {
                new GetEggsAction(),
                new GetMilkAction(),
                //new GetOilAction(),
                new ShakeEggsAction(),
                new FryOmeletteAction(),
            };

            var planner = new Planner();

            var plan = planner.MakePlan(state, goal, actions);

            Assert.IsNull(plan);
        }
    }
}

using GameplayCore;
using GameplayCore.AI;
using NUnit.Framework;
using System.Collections.Generic;

namespace EngineDotnetUnitTests.AI
{
    public class AIPlannerTests
    {
        public class OmeletteGoal : AIGoal
        {
            public override int DistanceTo(AIState state)
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
        }

        public class GetEggsAction : AIAction
        {
            public override bool CheckPreconditions(GameObject gameObject, AIState state)
            {
                return true;
            }

            public override IEnumerable<AIExecutionState> Execute(GameObject gameObject, AIState state)
            {
                state.SetBoolValue("HasEggs", true);
                yield return AIExecutionState.Finished;
            }

            public override void PlanEffects(GameObject gameObject, AIState state)
            {
                state.SetBoolValue("HasEggs", true);
            }
        }

        public class GetMilkAction : AIAction
        {
            public override bool CheckPreconditions(GameObject gameObject, AIState state)
            {
                return true;
            }

            public override IEnumerable<AIExecutionState> Execute(GameObject gameObject, AIState state)
            {
                state.SetBoolValue("HasMilk", true);
                yield return AIExecutionState.Finished;
            }

            public override void PlanEffects(GameObject gameObject, AIState state)
            {
                state.SetBoolValue("HasMilk", true);
            }
        }

        public class GetOilAction : AIAction
        {
            public override bool CheckPreconditions(GameObject gameObject, AIState state)
            {
                return true;
            }

            public override IEnumerable<AIExecutionState> Execute(GameObject gameObject, AIState state)
            {
                state.SetBoolValue("HasOil", true);
                yield return AIExecutionState.Finished;
            }

            public override void PlanEffects(GameObject gameObject, AIState state)
            {
                state.SetBoolValue("HasOil", true);
            }
        }

        public class ShakeEggsAction : AIAction
        {
            public override bool CheckPreconditions(GameObject gameObject, AIState state)
            {
                return state.GetBoolValue("HasEggs")
                    && state.GetBoolValue("HasMilk");
            }

            public override IEnumerable<AIExecutionState> Execute(GameObject gameObject, AIState state)
            {
                state.SetBoolValue("HasEggs", false);
                state.SetBoolValue("HasMilk", false);
                state.SetBoolValue("HasBeatenEggs", true);
                yield return AIExecutionState.Finished;
            }

            public override void PlanEffects(GameObject gameObject, AIState state)
            {
                state.SetBoolValue("HasEggs", false);
                state.SetBoolValue("HasMilk", false);
                state.SetBoolValue("HasBeatenEggs", true);
            }
        }

        public class FryOmeletteAction : AIAction
        {
            public override bool CheckPreconditions(GameObject gameObject, AIState state)
            {
                return state.GetBoolValue("HasOil")
                    && state.GetBoolValue("HasBeatenEggs");
            }

            public override IEnumerable<AIExecutionState> Execute(GameObject gameObject, AIState state)
            {
                state.SetBoolValue("HasOil", false);
                yield return AIExecutionState.InProgress;

                state.SetBoolValue("HasBeatenEggs", false);
                yield return AIExecutionState.InProgress;

                state.SetBoolValue("HasOmelette", true);
                yield return AIExecutionState.Finished;
            }

            public override void PlanEffects(GameObject gameObject, AIState state)
            {
                state.SetBoolValue("HasOil", false);
                state.SetBoolValue("HasBeatenEggs", false);
                state.SetBoolValue("HasOmelette", true);
            }
        }

        [Test]
        public void MakePlan_ActionsCanReachTheGoal_PlanIsMaked()
        {
            var state = new AIState();
            state.SetBoolValue("HasEggs", false);
            state.SetBoolValue("HasMilk", false);
            state.SetBoolValue("HasOil", false);
            state.SetBoolValue("HasBeatenEggs", false);
            state.SetBoolValue("HasOmelette", false);

            var goal = new OmeletteGoal();

            var actions = new List<AIAction>()
            {
                new GetEggsAction(),
                new GetMilkAction(),
                new GetOilAction(),
                new ShakeEggsAction(),
                new FryOmeletteAction(),
            };

            var planner = new AIPlanner();

            var plan = planner.MakePlan(null, state, goal, actions);

            foreach (var _ in plan.Execute(null, state));

            Assert.IsTrue(plan.ExecutionState == AIExecutionState.Finished);
            Assert.IsTrue(state.GetBoolValue("HasOmelette"));
        }

        [Test]
        public void MakePlan_ActionsCanNotReachTheGoal_PlanIsNull()
        {
            var state = new AIState();
            state.SetBoolValue("HasEggs", false);
            state.SetBoolValue("HasMilk", false);
            state.SetBoolValue("HasOil", false);
            state.SetBoolValue("HasBeatenEggs", false);
            state.SetBoolValue("HasOmelette", false);

            var goal = new OmeletteGoal();

            var actions = new List<AIAction>()
            {
                new GetEggsAction(),
                new GetMilkAction(),
                //new GetOilAction(),
                new ShakeEggsAction(),
                new FryOmeletteAction(),
            };

            var planner = new AIPlanner();

            var plan = planner.MakePlan(null, state, goal, actions);

            Assert.IsNull(plan);
        }
    }
}

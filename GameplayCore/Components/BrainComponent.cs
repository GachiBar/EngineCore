using GameplayCore.AI;
using GameplayCore.Resources;
using System.Collections.Generic;

namespace GameplayCore.Components
{
    public class BrainComponent : Component
    {
        public AIState _state;
        private AIPlanner _planner;
        private AIGoal _goal;
        private AIPlan _plan;
        private IEnumerator<AIExecutionState> _executionState;

        public AIArbitrator Arbitrator;
        public AIActionsAsset ActionsAsset;

        public BrainComponent()
        {
            _state = new AIState();
            _planner = new AIPlanner();
        }

        public override void Initialize()
        {
            if (ActionsAsset == null || Arbitrator == null)
            {
                return;
            }

            Arbitrator.InitState(_state);
            ActionsAsset.Load();

            if (ActionsAsset.Actions.Count == 0)
            {
                return;
            }

            _goal = Arbitrator.ChooseGoal(GameObject, _state);
            _plan = _planner.MakePlan(GameObject, _state, _goal, ActionsAsset.Actions);
            _executionState = _plan.Execute(GameObject, _state).GetEnumerator();
        }

        public override void UpdateAI()
        {
            if (ActionsAsset == null || Arbitrator == null || ActionsAsset.Actions.Count == 0)
            {
                return;
            }

            if (_plan != null && _plan.ExecutionState == AIExecutionState.InProgress)
            {
                return;
            }

            _goal = Arbitrator.ChooseGoal(GameObject, _state);
            _plan = _planner.MakePlan(GameObject, _state, _goal, ActionsAsset.Actions);
            _executionState = _plan.Execute(GameObject, _state).GetEnumerator();
        }

        public override void FixedUpdate()
        {
            if (_plan != null)
            {
                _executionState.MoveNext();
            }            
        }
    }
}

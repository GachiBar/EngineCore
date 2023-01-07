using GameplayCore.AI;
using GameplayCore.AI.Enemy;
using GameplayCore.AI.Enemy.Actions;
using GameplayCore.Resources;
using System.Collections.Generic;

namespace GameplayCore.Components
{
    public class BrainComponent : Component
    {
        private AIState _state;
        private AIPlanner _planner;
        private AIArbitrator _arbitrator;
        private List<AIAction> _actions;
        private AIGoal _goal;
        private AIPlan _plan;
        private IEnumerator<AIExecutionState> _executionState;

        public AIActionsAsset Actions;

        public BrainComponent()
        {
            _state = new AIState();
            _state.SetGameObjectValue("Aim", null);
            _state.SetBoolValue("HasAim", false);
            _state.SetBoolValue("CanShoot", false);
            _state.SetBoolValue("NeedReloading", false);
            _state.SetBoolValue("NeedFlee", false);
            _state.SetFloatValue("ReloadingTime", 0.75f);

            _planner = new AIPlanner();
            _arbitrator = new EnemyArbitrator();

            // Now we just hard code all actions.
            _actions = new List<AIAction>
            {
                new FindAimAction(),
                new TakeShootPositionAction(),
                new ShootAction(),
                new ReloadingAction(),
                new FleeAction(),
            };            
        }

        public override void Initialize()
        {
            _goal = _arbitrator.ChooseGoal(GameObject, _state);
            _plan = _planner.MakePlan(GameObject, _state, _goal, _actions);
            _executionState = _plan.Execute(GameObject, _state).GetEnumerator();
        }

        public override void UpdateAI()
        {
            if (_plan.ExecutionState != AIExecutionState.InProgress)
            {
                _goal = _arbitrator.ChooseGoal(GameObject, _state);
                _plan = _planner.MakePlan(GameObject, _state, _goal, _actions);
                _executionState = _plan.Execute(GameObject, _state).GetEnumerator();
            }
        }

        public override void FixedUpdate()
        {
            _executionState.MoveNext();
        }
    }
}

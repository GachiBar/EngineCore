using GameplayCore.AI;
using GameplayCore.AI.Enemy;
using GameplayCore.AI.Enemy.Actions;
using System;
using System.Collections.Generic;
using Action = GameplayCore.AI.Action;

namespace GameplayCore.Components
{
    public class BrainComponent : Component
    {
        private State _state;
        private Planner _planner;
        private Arbitrator _arbitrator;
        private List<Action> _actions;
        private Goal _goal;
        private Plan _plan;
        private IEnumerator<ExecutionState> _executionState;

        public BrainComponent()
        {
            _state = new State();
            _state.SetGameObjectValue("Aim", null);
            _state.SetBoolValue("HasAim", false);
            _state.SetBoolValue("CanShoot", false);
            _state.SetBoolValue("NeedReloading", false);
            _state.SetBoolValue("NeedFlee", false);
            _state.SetFloatValue("ReloadingTime", 0.75f);

            _planner = new Planner();
            _arbitrator = new EnemyArbitrator();

            // Now we just hard code all actions.
            _actions = new List<Action>
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
            if (_plan.ExecutionState != ExecutionState.InProgress)
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

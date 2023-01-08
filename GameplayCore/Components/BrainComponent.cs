﻿using GameplayCore.AI;
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
        private AIGoal _goal;
        private AIPlan _plan;
        private IEnumerator<AIExecutionState> _executionState;

        public AIActionsAsset ActionsAsset;

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
        }

        public override void Initialize()
        {
            if (ActionsAsset == null)
            {
                return;
            }

            ActionsAsset.Load();

            if (ActionsAsset.Actions.Count == 0)
            {
                return;
            }

            _goal = _arbitrator.ChooseGoal(GameObject, _state);
            _plan = _planner.MakePlan(GameObject, _state, _goal, ActionsAsset.Actions);
            _executionState = _plan.Execute(GameObject, _state).GetEnumerator();
        }

        public override void UpdateAI()
        {
            if (ActionsAsset == null || ActionsAsset.Actions.Count == 0)
            {
                return;
            }

            if (_plan != null && _plan.ExecutionState == AIExecutionState.InProgress)
            {
                return;
            }

            _goal = _arbitrator.ChooseGoal(GameObject, _state);
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

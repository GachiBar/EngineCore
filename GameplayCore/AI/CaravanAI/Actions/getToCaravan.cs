﻿using GameplayCore.Components;
using GameplayCore.Mathematics;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameplayCore.AI.CaravanAI.Actions
{
    public class getToCaravan : AIAction
    {
        public override bool CheckPreconditions(GameObject gameObject, AIState state)
        {
            return !state.GetBoolValue("isNearCaravan");
        }

        public override IEnumerable<AIExecutionState> Execute(GameObject gameObject, AIState state)
        {
            var enemyTransform = gameObject.GetComponent<TransformComponent>();
            var enemyController = gameObject.GetComponent<RobbersUnitController>();
            var rb = gameObject.GetComponent<RigidbodyComponent>();

            GameObject controller = gameObject.Scene.First<GameObject>(go => go.GetComponent<CaravanRobController>() != null);
            var caravan = controller.GetComponent<CaravanRobController>().getCaravanPosition();
            while (distanceTo(gameObject, caravan) > 2f && !state.GetBoolValue("isRetreating"))
            {
                enemyController.MoveInDirection(getPosProjectionXZ(caravan));
                if (enemyController.TargetGO != null) {
                    if (state.GetBoolValue("hasTarget")) yield return AIExecutionState.Interrupted;
                }
                if (!state.GetBoolValue("isRetreating"))
                    yield return AIExecutionState.InProgress;
                else yield return AIExecutionState.Interrupted;
            }
            rb.Velocity = Vector3.Zero;
            state.SetBoolValue("isNearCaravan", true);
            yield return AIExecutionState.Finished;
        }

        public override void PlanEffects(GameObject gameObject, AIState state)
        {
            state.SetBoolValue("isNearCaravan", true);
        }

        private float distanceTo(GameObject unit, GameObject target)
        {
            return (getPosProjectionXZ(unit) - getPosProjectionXZ(target)).Length();
        }

        private Vector3 getPosProjectionXZ(GameObject go)
        {
            var pos = go.GetComponent<TransformComponent>().Position;
            return new Vector3(pos.X, 0, pos.Z);
        }
    }
}
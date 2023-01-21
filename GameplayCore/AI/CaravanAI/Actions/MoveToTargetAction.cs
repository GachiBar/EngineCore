using GameplayCore.Components;
using GameplayCore.Mathematics;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace GameplayCore.AI.CaravanAI.Actions
{
    public class MoveToTargetAction : AIAction
    {
        public override bool CheckPreconditions(GameObject gameObject, AIState state)
        {
            return state.GetBoolValue("hasTarget") && !state.GetBoolValue("isNearTarget");
        }

        public override IEnumerable<AIExecutionState> Execute(GameObject gameObject, AIState state)
        {
            var enemyController = gameObject.GetComponent<RobbersUnitController>();
            var rb = gameObject.GetComponent<RigidbodyComponent>();

            var target = enemyController.TargetGO;
            if (target == null)
            {
                state.SetBoolValue("isNearTarget", false);
                state.SetBoolValue("isNearBaseTile", false);
                state.SetBoolValue("hasTarget", false);
                state.SetBoolValue("isIdle", true);
                yield return AIExecutionState.Interrupted;
            }
            float dist = 0;
            if (target != null)
                dist = distanceTo(gameObject, target);
            else
            {
                state.SetBoolValue("isNearTarget", false);
                state.SetBoolValue("isNearBaseTile", false);
                state.SetBoolValue("hasTarget", false);
                state.SetBoolValue("isIdle", true);
                yield return AIExecutionState.Interrupted;
            }
            while (dist > 1.5f && !state.GetBoolValue("isRetreating"))
            {
                if (state.GetBoolValue("isRetreating"))
                     yield return AIExecutionState.Interrupted;
                if (target != null)
                {
                    dist = distanceTo(gameObject, target);
                    enemyController.MoveInDirection(getPosProjectionXZ(target));
                }
                else
                {
                    state.SetBoolValue("isNearTarget", false);
                    state.SetBoolValue("isNearCaravan", false);
                    state.SetBoolValue("hasTarget", false);
                    state.SetBoolValue("isIdle", true);
                    yield return AIExecutionState.Interrupted;
                }
                if (state.GetBoolValue("isIdle")) yield return AIExecutionState.Interrupted;
                yield return AIExecutionState.InProgress;
            }
            rb.Velocity = Vector3.Zero;
            if (target == null)
            {
                state.SetBoolValue("isNearTarget", false);
                state.SetBoolValue("isNearCaravan", false);
                state.SetBoolValue("hasTarget", false);
                state.SetBoolValue("isIdle", true);
                yield return AIExecutionState.Interrupted;
            }
            state.SetBoolValue("isNearTarget", true);
            yield return AIExecutionState.Finished;
        }

        public override void PlanEffects(GameObject gameObject, AIState state)
        {
            state.SetBoolValue("isNearTarget", true);
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

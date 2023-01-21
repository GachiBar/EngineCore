using GameplayCore.Components;
using GameplayCore.Mathematics;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameplayCore.AI.CaravanAI.Actions
{
    public class IdleRoamAction : AIAction
    {
        public override bool CheckPreconditions(GameObject gameObject, AIState state)
        {
            return !state.GetBoolValue("hasTarget") && state.GetBoolValue("isIdle");
        }

        public override IEnumerable<AIExecutionState> Execute(GameObject gameObject, AIState state)
        {
            var enemyTransform = gameObject.GetComponent<TransformComponent>();
            var enemyController = gameObject.GetComponent<RobbersUnitController>();
            var rb = gameObject.GetComponent<RigidbodyComponent>();
            GameObject controller = gameObject.Scene.First<GameObject>(go => go.GetComponent<CaravanRobController>() != null);
            var caravan = controller.GetComponent<CaravanRobController>().getCaravanPosition();
            while (distanceTo(gameObject, caravan) > 1f && !state.GetBoolValue("isRetreating") && !state.GetBoolValue("hasTarget"))
            {
                if (state.GetBoolValue("isRetreating"))
                    yield return AIExecutionState.Interrupted;
                if (distanceTo(gameObject, caravan) < 1.6f)
                {
                    caravan = controller.GetComponent<CaravanRobController>().getCaravanPosition();
                }
                enemyController.MoveInDirection(getPosProjectionXZ(caravan));
                if (enemyController.TargetGO != null)
                {
                    state.SetBoolValue("hasTarget", true);
                    state.SetBoolValue("isNearTarget", false);
                    state.SetBoolValue("isIdle", false);
                    Console.WriteLine("Found Target Enemy");
                    yield return AIExecutionState.Interrupted;
                }

                yield return AIExecutionState.InProgress;
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

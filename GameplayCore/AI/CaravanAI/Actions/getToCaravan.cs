using GameplayCore.Components;
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
            while (distanceTo(gameObject, caravan) > 3f )
            {
                enemyController.MoveInDirection(getPosProjectionXZ(caravan));
                Console.WriteLine("V1: {0}", distanceTo(gameObject, caravan));
                yield return AIExecutionState.InProgress;
            }
            rb.Velocity = Vector3.Zero;
            Console.WriteLine("V1: {0}", distanceTo(gameObject, caravan));
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

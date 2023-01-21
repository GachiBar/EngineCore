using GameplayCore.Components;
using GameplayCore.Mathematics;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameplayCore.AI.CaravanAI.Actions
{
    public class HideGoldAction : AIAction
    {
        public override bool CheckPreconditions(GameObject gameObject, AIState state)
        {
            return state.GetBoolValue("isNearCaravan") && state.GetBoolValue("collectedGold");
        }

        public override IEnumerable<AIExecutionState> Execute(GameObject gameObject, AIState state)
        {
            //go to base an leave gold there
            var enemyTransform = gameObject.GetComponent<TransformComponent>();
            var enemyController = gameObject.GetComponent<RobbersUnitController>();
            GameObject goBase = gameObject.Scene.First<GameObject>(go => go.GetComponent<BaseComponent>() != null);

            while (distanceToBase(gameObject, goBase) > 0.3f)
            {
                enemyController.MoveInDirection(getPosProjectionXZ(goBase));
                yield return AIExecutionState.InProgress;
            }
            var rb = gameObject.GetComponent<RigidbodyComponent>();
            rb.Velocity = Vector3.Zero;
            for (float timer = 0; timer < 1.5; timer += Time.FixedDeltaTime)
            {
                if (!state.GetBoolValue("isRetreating"))
                    yield return AIExecutionState.InProgress;
                else yield return AIExecutionState.Interrupted;
            }
            var gameController = gameObject.Scene.First<GameObject>(go => go.GetComponent<CaravanRobController>() != null).GetComponent<CaravanRobController>();
            gameController.addGold();
            state.SetBoolValue("collectedGold", false);
            state.SetBoolValue("isNearCaravan", false);
            state.SetBoolValue("reachedBase", true);
            yield return AIExecutionState.Finished;
        }

        public override void PlanEffects(GameObject gameObject, AIState state)
        {
            state.SetBoolValue("collectedGold", false); 
            state.SetBoolValue("isNearCaravan", false); 
            state.SetBoolValue("reachedBase", true);
        }


        private float distanceToBase(GameObject unit, GameObject baseObj)
        {
            return (getPosProjectionXZ(unit) - getPosProjectionXZ(baseObj)).Length();
        }

        private Vector3 getPosProjectionXZ(GameObject go)
        {
            var pos = go.GetComponent<TransformComponent>().Position;
            return new Vector3(pos.X, 0, pos.Z);
        }
    }
}

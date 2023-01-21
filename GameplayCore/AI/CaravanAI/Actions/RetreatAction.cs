using GameplayCore.Components;
using GameplayCore.Mathematics;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameplayCore.AI.CaravanAI.Actions
{
    public class RetreatAction : AIAction
    {
        public event Action<GameObject> Retreated;
        public override bool CheckPreconditions(GameObject gameObject, AIState state)
        {
            return state.GetBoolValue("isRetreating") && !state.GetBoolValue("reachedBase");
        }

        public override IEnumerable<AIExecutionState> Execute(GameObject gameObject, AIState state)
        {
            var enemyTransform = gameObject.GetComponent<TransformComponent>();
            var enemyController = gameObject.GetComponent<RobbersUnitController>();
            GameObject goBase = gameObject.Scene.First<GameObject>(go => go.GetComponent<BaseComponent>() != null);

            while (distanceToBase(gameObject, goBase) > 0.5f)
            {
                enemyController.MoveInDirection(getPosProjectionXZ(goBase));
                yield return AIExecutionState.InProgress;
            }
            var rb = gameObject.GetComponent<RigidbodyComponent>();
            rb.Velocity = Vector3.Zero;

            var gameController = gameObject.Scene.First<GameObject>(go => go.GetComponent<CaravanRobController>() != null).GetComponent<CaravanRobController>();
            if (state.TryGetBoolValue("collectedGold", out var res))
            {
                if (state.GetBoolValue("collectedGold"))
                {
                    gameController.addGold();
                }
            }
            //Retreated?.Invoke(gameObject);
            gameController.unitReturnToBase(gameObject);
            state.SetBoolValue("reachedBase", true);
            state.SetBoolValue("isRetreating", false);
            state.SetBoolValue("collectedGold", false);
            yield return AIExecutionState.Finished;
        }

        public override void PlanEffects(GameObject gameObject, AIState state)
        {
            state.SetBoolValue("reachedBase", true);
            state.SetBoolValue("isRetreating", false);
            state.SetBoolValue("collectedGold", false);
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

using GameplayCore.Components;
using GameplayCore.Mathematics;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameplayCore.AI.CaravanAI.Actions
{
    internal class RetreatAction : AIAction
    {
        public override bool CheckPreconditions(GameObject gameObject, AIState state)
        {
            return state.GetBoolValue("isRetreating") && !state.GetBoolValue("reachedBase");
        }

        public override IEnumerable<AIExecutionState> Execute(GameObject gameObject, AIState state)
        {
            var enemyTransform = gameObject.GetComponent<TransformComponent>();
            var enemyController = gameObject.GetComponent<RobbersUnitController>();
            GameObject goBase = gameObject.Scene.First<GameObject>(go => go.GetComponent<BaseComponent>() != null);

            while (distanceToBase(gameObject, goBase) > 3f)
            {
                enemyController.MoveInDirection(getPosProjectionXZ(goBase));
                yield return AIExecutionState.InProgress;
            }
            var rb = gameObject.GetComponent<RigidbodyComponent>();
            rb.Velocity = Vector3.Zero;
            state.SetBoolValue("reachedBase", true);
            state.SetBoolValue("isRetreating", false);
            //TODO: remove unit that reached base in flee mode 
            yield return AIExecutionState.Finished;
        }

        public override void PlanEffects(GameObject gameObject, AIState state)
        {
            state.SetBoolValue("reachedBase", true);
            state.SetBoolValue("isRetreating", false);
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

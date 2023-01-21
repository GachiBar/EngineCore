using GameplayCore.Components;
using GameplayCore.Mathematics;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameplayCore.AI.CaravanAI.Actions
{
    internal class GuardIdleAction : AIAction
    {
        public override bool CheckPreconditions(GameObject gameObject, AIState state)
        {
            return !state.GetBoolValue("hasTarget") && state.GetBoolValue("isIdle");
        }

        public override IEnumerable<AIExecutionState> Execute(GameObject gameObject, AIState state)
        {
            var enemyTransform = gameObject.GetComponent<TransformComponent>();
            var guardController = gameObject.GetComponent<GuardComponent>();
            var rb = gameObject.GetComponent<RigidbodyComponent>();
            GameObject controller = gameObject.Scene.First<GameObject>(go => go.GetComponent<CaravanRobController>() != null);
            List<GameObject> baseTiles = gameObject.Scene.OfType<GameObject>().Where(go => go.GetComponent<BlockpostComponent>() != null).ToList();
            Random r = new Random();
            GameObject closestBase = baseTiles.ToList()[r.Next(baseTiles.Count())];
            while (distanceTo(gameObject, closestBase) > 0.9f && !state.GetBoolValue("hasTarget"))
            {
                if (distanceTo(gameObject, closestBase) < 1.8f) {
                    List<GameObject> temp = new List<GameObject>(baseTiles);
                    temp.Remove(closestBase);
                    closestBase = temp[r.Next(temp.Count())];
                }
                guardController.MoveInDirection(getPosProjectionXZ(closestBase));
                if (guardController.TargetGO != null)
                {
                    state.SetBoolValue("hasTarget", true);
                    state.SetBoolValue("isNearTarget", false);
                    state.SetBoolValue("isIdle", false);
                    Console.WriteLine("Found Target");
                    yield return AIExecutionState.Interrupted;
                }
                yield return AIExecutionState.InProgress;
            }
            rb.Velocity = Vector3.Zero;
            state.SetBoolValue("isNearBaseTile", true);
            yield return AIExecutionState.Finished;
        }

        public override void PlanEffects(GameObject gameObject, AIState state)
        {
            state.SetBoolValue("isNearBaseTile", true);
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

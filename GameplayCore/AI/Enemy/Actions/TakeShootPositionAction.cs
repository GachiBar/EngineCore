using GameplayCore.Components;
using GameplayCore.Mathematics;
using System;
using System.Collections.Generic;

namespace GameplayCore.AI.Enemy.Actions
{
    public class TakeShootPositionAction : AIAction
    {
        public override bool CheckPreconditions(GameObject gameObject, AIState state)
        {
            return state.GetBoolValue("HasAim") == true;
        }

        public override IEnumerable<AIExecutionState> Execute(GameObject gameObject, AIState state)
        {
            var aim = state.GetGameObjectValue("Aim");
            var enemyTransform = gameObject.GetComponent<TransformComponent>();
            var enemyController = gameObject.GetComponent<RangeEnemyControllerComponent>();
            var aimTransform = aim.GetComponent<TransformComponent>();

            while (true)
            {
                bool isReadyToShoot = true;
                var viewDirection = GetViewDirection(enemyTransform.Position, aimTransform.Position);

                if (IsAimInRange(enemyTransform.Position, aimTransform.Position, enemyController.Range))
                {
                    enemyController.MoveInDirection(viewDirection);                   
                    isReadyToShoot = false;
                }

                if (!IsAimInViewSector(enemyTransform.Forward, viewDirection, enemyController.ViewSector))
                {
                    enemyController.LookInDirection(viewDirection);
                    isReadyToShoot = false;
                }

                if (isReadyToShoot)
                {
                    break;
                }

                yield return AIExecutionState.InProgress;
            }

            state.SetBoolValue("CanShoot", true);
            yield return AIExecutionState.Finished;
        }

        public override void PlanEffects(GameObject gameObject, AIState state)
        {
            state.SetBoolValue("CanShoot", true);
        }

        private Vector3 GetViewDirection(Vector3 from, Vector3 to)
        {
            var axis = Vector3.UnitX + Vector3.UnitZ;
            var direction = to - from;
            var viewDirection = direction * axis;
            viewDirection.Normalize();
            return viewDirection;
        }

        private bool IsAimInRange(Vector3 enemyPosition, Vector3 aimPosition, float range)
        {
            var direction = enemyPosition - aimPosition;
            return direction.Length() > range;
        }

        private bool IsAimInViewSector(Vector3 forward, Vector3 direction, float viewSector)
        {
            float cosa = MathUtil.Clamp(Vector3.Dot(forward, direction), -1, 1);
            float angle = (float)Math.Abs(MathUtil.RadiansToDegrees((float)Math.Acos(cosa)));
            return cosa > 0 && angle < viewSector;
        }
    }
}

using GameplayCore.Mathematics;
using System;

namespace GameplayCore.Components
{
    public class RangeEnemyControllerComponent : Component
    {
        private const float BulletRadius = 0.3f;

        public float Speed = 2.0f;
        public float Range = 2.0f;
        
        public GameObject BulletInitialPoint;
        public float BulletImpulse = 50.0f;
        public float BulletLifeTime = 2.0f;

        public void MoveInDirection(Vector3 direction)
        {
            var transform = GameObject.GetComponent<TransformComponent>();
            var rigidbody = GameObject.GetComponent<RigidbodyComponent>();
            
            if (transform == null)
            {
                Log.PrintError($"GameObject does not contains {nameof(TransformComponent)}");
                return;
            }
            if (rigidbody == null)
            {
                Log.PrintError($"GameObject does not contains {nameof(RigidbodyComponent)}");
                return;
            }

            var velocity = rigidbody.Velocity * Vector3.UnitY;
            velocity += direction * Speed;
            rigidbody.Velocity = velocity;

            transform.Rotation *= GetRotation(transform.Forward, direction);
        }

        public void LoockInDirection(Vector3 direction)
        {
            var transform = GameObject.GetComponent<TransformComponent>();
            transform.Rotation *= GetRotation(transform.Forward, direction);
        }

        public void Shoot(GameObject aim)
        {
            var initialPointTransform = BulletInitialPoint.GetComponent<TransformComponent>();
            var aimTransform = aim.GetComponent<TransformComponent>();

            if (initialPointTransform == null)
            {
                Log.PrintError($"Initial point has not {nameof(TransformComponent)}!");
                return;
            }
            if (aimTransform == null)
            {
                Log.PrintError($"Aim has not {nameof(TransformComponent)}!");
                return;
            }

            var position = initialPointTransform.Position;
            var destination = aimTransform.Position;
            var direction = destination - position;
            direction.Normalize();
            ThrowBullet(position, direction);
        }

        private void ThrowBullet(Vector3 position, Vector3 direction)
        {
            var bullet = GameObject.Scene.CreateGameObject();
            var bulletTransform = bullet.AddComponent<TransformComponent>();
            var bulletRigidbody = bullet.AddComponent<RigidbodyComponent>();
            var bulletSphere = bullet.AddComponent<SphereCollisionComponent>();
            var bulletTimer = bullet.AddComponent<TimerDestroyerComponent>();

            bulletTransform.Position = position;
            bulletSphere.Radius = BulletRadius;
            bulletTimer.Timer = BulletLifeTime;
            bulletRigidbody.Mass = 1.0f;
            bulletRigidbody.AddImpulse(direction * BulletImpulse);
        }

        private static Quaternion GetRotation(Vector3 from, Vector3 to)
        {
            from.Normalize();
            to.Normalize();

            if (from == to)
            {
                return Quaternion.Identity;
            }

            float cosa = MathUtil.Clamp(Vector3.Dot(from, to), -1, 1);
            var axis = Vector3.Cross(from, to);
            float angle = (float)Math.Acos(cosa);

            var rotation = Quaternion.RotationAxis(axis, angle);
            rotation.Normalize();
            return rotation;
        }
    }
}

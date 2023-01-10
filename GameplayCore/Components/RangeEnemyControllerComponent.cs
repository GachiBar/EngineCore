using GameplayCore.Mathematics;
using GameplayCore.Resources;
using System;

namespace GameplayCore.Components
{
    public class RangeEnemyControllerComponent : Component
    {        
        private const float BulletRadius = 0.3f;

        private HealthComponent _healthComponent;

        public float Speed = 2.0f;
        public float AngularSpeed = 4.0f;
        public float Range = 4.0f;
        public float ViewSector = 20.0f;
        
        public GameObject BulletInitialPoint;
        public float BulletImpulse = 50.0f;
        public float BulletLifeTime = 2.0f;
        public MeshAsset BulletMesh;

        public void MoveInDirection(Vector3 direction)
        {
            var transform = GameObject.GetComponent<TransformComponent>();
            var rigidbody = GameObject.GetComponent<RigidbodyComponent>();
            
            if (transform == null)
            {
                Log.PrintError($"GameObject: {GameObject.Name} does not contains {nameof(TransformComponent)}");
                return;
            }
            if (rigidbody == null)
            {
                Log.PrintError($"GameObject: {GameObject.Name} does not contains {nameof(RigidbodyComponent)}");
                return;
            }

            var velocity = rigidbody.Velocity * Vector3.UnitY;
            velocity += direction * Speed;
            rigidbody.Velocity = velocity;
        }

        public void Stay()
        {
            var rigidbody = GameObject.GetComponent<RigidbodyComponent>();

            if (rigidbody == null)
            {
                Log.PrintError($"GameObject: {GameObject.Name} does not contains {nameof(RigidbodyComponent)}");
                return;
            }

            rigidbody.Velocity *= Vector3.Up;
        }

        public void LoockInDirection(Vector3 direction)
        {
            var transform = GameObject.GetComponent<TransformComponent>();

            if (transform == null)
            {
                Log.PrintError($"GameObject: {GameObject.Name} does not contains {nameof(TransformComponent)}");
                return;
            }

            LoockInDirection(transform, direction);
        }

        public void Reload(float progress, float max)
        {
            var transform = GameObject.GetComponent<TransformComponent>();

            if (transform == null)
            {
                Log.PrintError($"GameObject: {GameObject.Name} does not contains {nameof(TransformComponent)}");
                return;
            }

            float amount = (float)Math.Sin(progress / max * MathUtil.Pi);
            amount = MathUtil.Clamp(amount, 0, 1);
            float scale = MathUtil.Lerp(1.0f, 0.8f, amount);
            transform.Scale = new Vector3(scale);
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

        protected override void OnAttach(GameObject gameObject)
        {
            _healthComponent = gameObject.GetComponent<HealthComponent>();

            if (_healthComponent != null)
            {
                _healthComponent.Died += OnDied;
            }

            gameObject.ComponentAdded += OnComponentAdded;
            gameObject.ComponentRemoved += OnComponentRemoved;
        }

        protected override void OnDetach(GameObject gameObject)
        {
            gameObject.ComponentAdded -= OnComponentAdded;
            gameObject.ComponentRemoved -= OnComponentRemoved;
        }

        private void OnComponentAdded(GameObject gameObject, Component component)
        {
            if (component is HealthComponent healthComponent)
            {
                _healthComponent = healthComponent;
                _healthComponent.Died += OnDied;
            }
        }

        private void OnComponentRemoved(GameObject gameObject, Component component)
        {
            if (component is HealthComponent)
            {
                _healthComponent.Died -= OnDied;
                _healthComponent = null;
            }
        }

        private void LoockInDirection(TransformComponent transform, Vector3 direction)
        {
            var from = transform.Rotation;
            var to = from * GetRotation(transform.Forward, direction);
            var rotation = Quaternion.Lerp(from, to, AngularSpeed * Time.FixedDeltaTime);
            transform.Rotation = rotation;         
        }

        private void ThrowBullet(Vector3 position, Vector3 direction)
        {
            var bullet = GameObject.Scene.CreateGameObject();
            var bulletTransform = bullet.AddComponent<TransformComponent>();
            var bulletMesh = bullet.AddComponent<MeshRenderComponent>();
            var bulletRigidbody = bullet.AddComponent<RigidbodyComponent>();
            var bulletSphere = bullet.AddComponent<SphereCollisionComponent>();
            var bulletTimer = bullet.AddComponent<TimerDestroyerComponent>();

            bulletTransform.Position = position;
            bulletTransform.Scale = new Vector3(BulletRadius);
            bulletMesh.MeshAsset = BulletMesh;
            bulletSphere.Radius = BulletRadius;
            bulletTimer.Timer = BulletLifeTime;
            bulletRigidbody.Mass = 1.0f;
            bulletRigidbody.AddImpulse(direction * BulletImpulse);
        }

        private void OnDied(HealthComponent health)
        {
            // Now I just remove game object
            GameObject.Scene.DeleteGameObject(GameObject);
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

using GameplayCore.Mathematics;

namespace GameplayCore.Components
{
    public class FpsControllerComponent : Component
    {
        private const float Weight = 0.03f;
        private const float BulletRadius = 0.3f;

        private RigidbodyComponent _rigidbodyComponent;        

        private Vector3 _axis;
        private float _yaw = 0.0f;
        private float _pitch = 0.0f;
        
        public float Speed = 3.0f;
        public float Sensivity = 1.0f;
        public float BulletImpulse = 10000.0f;
        public float BulletLifeTime = 2.0f;
        public GameObject Head;

        public override void Update()
        {         
            var headTransform = Head.GetComponent<TransformComponent>();

            if (headTransform != null)
            {
                var dx = Input.GetKeyValue(Keys.MouseX);
                var dy = Input.GetKeyValue(Keys.MouseY);

                _yaw -= dx * Weight * Sensivity;
                _pitch -= dy * Weight * Sensivity;
                _pitch = MathUtil.Clamp(_pitch, -MathUtil.PiOverTwo, MathUtil.PiOverTwo);
                _axis = GetAxis();

                var rotation = Quaternion.RotationYawPitchRoll(_yaw, _pitch, 0);
                headTransform.LocalRotation = rotation;

                if (Input.WasJustPressed(Keys.LeftMouseButton))
                {
                    var bullet = GameObject.Scene.CreateGameObject();
                    var bulletTransform = bullet.AddComponent<TransformComponent>();                    
                    var bulletRigidbody = bullet.AddComponent<RigidbodyComponent>();
                    var bulletSphere = bullet.AddComponent<SphereCollisionComponent>();
                    var bulletTimer = bullet.AddComponent<TimerDestroyerComponent>();

                    bulletTransform.Position = headTransform.Position + headTransform.Forward * 0.5f;
                    bulletSphere.Radius = BulletRadius;
                    bulletTimer.Timer = BulletLifeTime;
                    bulletRigidbody.AddImpulse(headTransform.Forward * BulletImpulse);
                }
            }            
        }

        public override void FixedUpdate()
        {
            if (_rigidbodyComponent != null)
            {
                var headTransform = Head.GetComponent<TransformComponent>();

                if (headTransform != null)
                {
                    var rotation = Quaternion.RotationYawPitchRoll(_yaw, 0, 0);
                    var direction = Vector3.Transform(_axis, rotation);
                    direction.Normalize();
                    _rigidbodyComponent.Velocity = direction * Speed;
                }                
            }
        }

        protected override void OnAttach(GameObject gameObject)
        {
            _rigidbodyComponent = gameObject.GetComponent<RigidbodyComponent>();
            
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
            if (component is RigidbodyComponent rigidbodyComponent)
            {
                _rigidbodyComponent = rigidbodyComponent;
            }
        }

        private void OnComponentRemoved(GameObject gameObject, Component component)
        {
            if (component is RigidbodyComponent)
            {
                _rigidbodyComponent = null;
            }
        }

        private Vector3 GetAxis()
        {
            var direction = Vector3.Zero;

            if (Input.IsPressed(Keys.A))
            {
                direction -= Vector3.UnitX;
            }
            if (Input.IsPressed(Keys.D))
            {
                direction += Vector3.UnitX;
            }
            if (Input.IsPressed(Keys.W))
            {
                direction -= Vector3.UnitZ;
            }
            if (Input.IsPressed(Keys.S))
            {
                direction += Vector3.UnitZ;
            }

            return direction;
        }
    }
}

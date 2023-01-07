using GameplayCore.Mathematics;

namespace GameplayCore.Components
{
    public class SpectatorComponent : Component
    {
        private TransformComponent _transformComponent;
        private float _weight = 0.03f;
        private float _yaw = 0.0f;
        private float _pitch = 0.0f;

        public float Velocity = 2.0f;
        public float Sensivity = 1.0f;

        public override void Update()
        {
            if (_transformComponent == null)
            {
                Log.PrintWarning($"{nameof(SpectatorComponent)} requier {nameof(TransformComponent)}.");
                return;
            }

            var dx = Input.GetKeyValue(Keys.MouseX);
            var dy = Input.GetKeyValue(Keys.MouseY);

            _yaw -= dx * _weight * Sensivity;
            _pitch -= dy * _weight * Sensivity;
            _pitch = MathUtil.Clamp(_pitch, -MathUtil.PiOverTwo, MathUtil.PiOverTwo);

            var rotation = Matrix.RotationYawPitchRoll(_yaw, _pitch, 0);
            var axis = GetAxis();
            var direction =
                axis.X * rotation.Right +
                axis.Y * Vector3.Up +
                axis.Z * rotation.Forward;

            direction.Normalize();
            _transformComponent.LocalPosition += direction * Velocity * Time.DeltaTime;
            _transformComponent.LocalRotation = Quaternion.RotationMatrix(rotation);
        }

        protected override void OnAttach(GameObject gameObject)
        {
            _transformComponent = gameObject.GetComponent<TransformComponent>();

            if (_transformComponent == null)
            {
                Log.PrintWarning($"{nameof(SpectatorComponent)} requier {nameof(TransformComponent)}.");
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
            if (component is TransformComponent transformComponent)
            {
                _transformComponent = transformComponent;
            }
        }

        private void OnComponentRemoved(GameObject gameObject, Component component)
        {
            if (component is TransformComponent)
            {
                _transformComponent = null;
                Log.PrintWarning($"{nameof(SpectatorComponent)} requier {nameof(TransformComponent)}.");
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
                direction += Vector3.UnitZ;
            }
            if (Input.IsPressed(Keys.S))
            {
                direction -= Vector3.UnitZ;
            }
            if (Input.IsPressed(Keys.SpaceBar))
            {
                direction += Vector3.UnitY;
            }
            if (Input.IsPressed(Keys.LeftShift))
            {
                direction -= Vector3.UnitY;
            }

            return direction;
        }
    }
}

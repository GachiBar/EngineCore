using GameplayCore.Mathematics;
using System;
using System.CodeDom;

namespace GameplayCore.Components
{
    public class SpectatorComponent : Component
    {
        private TransformComponent _transformComponent;
        private float _weight = 0.03f;
        private float _yaw = 0.0f;
        private float _pitch = 0.0f;

        public float Velocity { get; set; } = 2.0f;
        public float Sensivity { get; set; } = 1.0f;

        public override void Update()
        {
            var dx = Input.GetKeyValue("MouseX");
            var dy = Input.GetKeyValue("MouseY");

            _yaw -= dx * _weight * Sensivity;
            _pitch -= dy * _weight * Sensivity;
            _pitch = MathUtil.Clamp(_pitch, -MathUtil.PiOverTwo, MathUtil.PiOverTwo);

            var rotation = Matrix.RotationYawPitchRoll(_yaw, _pitch, 0);
            var direction = GetDirection();
            direction = 
                direction.X * rotation.Right + 
                direction.Y * Vector3.Up + 
                direction.Z * rotation.Forward;

            _transformComponent.Position += direction * Velocity * Time.DeltaTime;          
            _transformComponent.Rotation = Quaternion.RotationMatrix(rotation);

            Console.WriteLine($"x: {dx} y: {dy}");
        }

        protected override void OnAttach(GameObject gameObject)
        {
            _transformComponent = gameObject.GetComponent<TransformComponent>();

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
            }
        }

        private Vector3 GetDirection()
        {
            var direction = Vector3.Zero;

            if (Input.IsPressed("A"))
            {
                direction += Vector3.Left;
            }
            if (Input.IsPressed("D"))
            {
                direction += Vector3.Right;
            }
            // TODO: why -?
            if (Input.IsPressed("W"))
            {
                direction -= Vector3.ForwardRH;
            }
            // TODO: why -?
            if (Input.IsPressed("S"))
            {
                direction -= Vector3.BackwardRH;
            }
            if (Input.IsPressed("SpaceBar"))
            {
                direction += Vector3.Up;
            }
            if (Input.IsPressed("LeftShift"))
            {
                direction += Vector3.Down;
            }

            return direction;
        }
    }
}

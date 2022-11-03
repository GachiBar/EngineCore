using GameplayCore.Mathematics;

namespace GameplayCore.Components
{
    public class CameraComponent : Component
    {
        private float _fow = MathUtil.PiOverTwo;
        private TransformComponent _transformComponent = null;

        public float FieldOfView 
        { 
            get => MathUtil.RadiansToDegrees(_fow); 
            set => _fow = MathUtil.DegreesToRadians(value); 
        }

        public float Near { get; set; } = 0.1f;
        public float Far { get; set; } = 100.0f;

        public override void Render()
        {
            if (_transformComponent != null)
            {
                var fow = _fow;
                var eye = _transformComponent.Position;
                var target = _transformComponent.Position + _transformComponent.Forward;
                var up = _transformComponent.Up;
                var view = Matrix.LookAtRH(eye, target, up);
                var projection = Matrix.PerspectiveFovRH(fow, 800.0f / 600, Near, Far);
                EngineApi.Render.SetViewProjection(Time.EllapsedTime, view, projection);
            }
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
    }
}

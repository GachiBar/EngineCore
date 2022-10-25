using GameplayCore.Mathematics;

namespace GameplayCore.Components
{
    public class CameraComponent : Component
    {
        private TransformComponent _transformComponent = null;

        public float FieldOfView { get; set; } = 90.0f;
        public float Near { get; set; } = 0.1f;
        public float Far { get; set; } = 100.0f;

        public override void Initialize()
        {
            _transformComponent.Position = 5 * Vector3.BackwardRH;
        }

        public override void Render()
        {
            if (_transformComponent != null)
            {
                var fow = MathUtil.DegreesToRadians(FieldOfView);
                var eye = _transformComponent.Position;
                var target = _transformComponent.Forward;
                var view = Matrix.LookAtRH(eye, target, Vector3.Up);
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

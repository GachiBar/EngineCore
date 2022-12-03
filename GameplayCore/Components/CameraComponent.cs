using GameplayCore.Mathematics;

namespace GameplayCore.Components
{
    public class CameraComponent : Component
    {
        public float FieldOfView = 90;
        public float Near = 0.1f;
        public float Far = 100.0f;

        private TransformComponent _transformComponent = null;      

        public override void Update()
        {
            if (_transformComponent != null)
            {
                var fow = MathUtil.DegreesToRadians(FieldOfView);
                var eye = _transformComponent.Position;
                var target = _transformComponent.Position + _transformComponent.Forward;
                var up = _transformComponent.Up;
                var view = Matrix.LookAtRH(eye, target, up);
                float aspect = (float)Screen.Width / Screen.Height;
                var projection = Matrix.PerspectiveFovRH(fow, aspect, Near, Far);
                EngineApi.RenderApi.SetViewProjection(Time.EllapsedTime, view, projection);
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

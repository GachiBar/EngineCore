using GameplayCore.EngineApi;
using GameplayCore.Mathematics;

namespace GameplayCore.Components
{
    public class DirectionalLightComponent : Component
    {
        private TransformComponent _transformComponent;

        public float Intencity;
        public Vector3 Color;

        public override void Render()
        {
            if (_transformComponent != null)
            {
                RenderApi.DrawDirectionalLight(_transformComponent.Forward, Intencity, Color);
            }
        }

        public override void DebugRender()
        {
            if (_transformComponent != null)
            {
                var model = Matrix.Identity;
                model *= Matrix.RotationQuaternion(_transformComponent.Rotation);
                model *= Matrix.Translation(_transformComponent.Position);

                Gizmos.DrawLine(
                    model,
                    Vector3.Zero, 
                    Vector3.ForwardRH * 0.5f, 
                    new Vector3(0.0f, 255.0f, 255.0f));
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

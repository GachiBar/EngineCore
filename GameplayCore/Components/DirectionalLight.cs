using GameplayCore.EngineApi;
using GameplayCore.Mathematics;

namespace GameplayCore.Components
{
    public class DirectionalLightComponent : Component
    {
        private const int PlainPointsCount = 4;

        private static readonly Vector3 GizmosColor;        
        private static readonly Vector3[] ArrowPlane;

        private static readonly float SphereRadius;
        private static readonly float Distance;

        private TransformComponent _transformComponent;

        public float Intencity;
        public Vector3 Color;

        static DirectionalLightComponent()
        {
            GizmosColor = new Vector3(255.0f, 255.0f, 0.0f);
            SphereRadius = 0.2f;
            Distance = 0.5f;

            float centerOffset = -0.3f;
            float halfEdge = 0.05f;
           
            ArrowPlane = new Vector3[PlainPointsCount + 1]
            {
                new Vector3(halfEdge, halfEdge, centerOffset),
                new Vector3(-halfEdge, halfEdge, centerOffset),
                new Vector3(-halfEdge, -halfEdge, centerOffset),
                new Vector3(halfEdge, -halfEdge, centerOffset),
                new Vector3(halfEdge, halfEdge, centerOffset),
            };
        }

        public override void Render()
        {
            if (_transformComponent != null)
            {
                RenderApi.DrawDirectionalLight(_transformComponent.Forward, Intencity, Color);
            }
        }

        public override void EditorRender()
        {
            if (_transformComponent != null)
            {
                var model = Matrix.Identity;
                model *= Matrix.RotationQuaternion(_transformComponent.Rotation);
                model *= Matrix.Translation(_transformComponent.Position);

                Gizmos.DrawSphere(model, SphereRadius, GizmosColor);
                Gizmos.DrawLine(
                    model,
                    Vector3.ForwardRH * SphereRadius, 
                    Vector3.ForwardRH * Distance, 
                    GizmosColor);
                Gizmos.DrawCurve(model, ArrowPlane, GizmosColor);

                for (int i = 0; i < PlainPointsCount; i++)
                {
                    Gizmos.DrawLine(model, ArrowPlane[i], Vector3.ForwardRH * Distance, GizmosColor);
                }
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

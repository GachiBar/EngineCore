using GameplayCore.Editor;
using GameplayCore.EngineApi;
using GameplayCore.Mathematics;
using GameplayCore.Physics;
using GameplayCore.Serialization;

namespace GameplayCore.Components
{
    public class SphereBodyComponent : BodyComponent
    {
        private TransformComponent _transformComponent;
        
        [SerializeField]
        [InspectorName("Radius")]
        private float _radius = 1.0f;

        internal override void Invalidate(string fieldName)
        {
            base.Invalidate(fieldName);

            switch (fieldName)
            {
                case nameof(_radius):
                    PhysicsApi.SetSphereShape(BodyId, _radius);
                    break;
            }
        }

        protected override uint CreateBody(Vector3 position, Quaternion rotation, MotionType motionType, CollisionLayer collisionLayer)
        {
            return PhysicsApi.CreateSphereBody(_radius, position, rotation, motionType, collisionLayer);
        }

        protected override void RenderCollider()
        {
            Gizmos.DrawSphere(
                _transformComponent.Position, 
                _transformComponent.Rotation, 
                Vector3.One,
                _radius, 
                new Vector3(0.0f, 255.0f, 0.0f));
        }

        protected override void OnAttach(GameObject gameObject)
        {
            base.OnAttach(gameObject);
            _transformComponent = GameObject.GetComponent<TransformComponent>();

            gameObject.ComponentAdded += OnComponentAdded;
            gameObject.ComponentRemoved += OnComponentRemoved;
        }

        protected override void OnDetach(GameObject gameObject)
        {
            base.OnDetach(gameObject);
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

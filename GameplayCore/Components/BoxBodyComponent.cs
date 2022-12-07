using GameplayCore.Editor;
using GameplayCore.EngineApi;
using GameplayCore.Mathematics;
using GameplayCore.Physics;
using GameplayCore.Serialization;

namespace GameplayCore.Components
{
    public class BoxBodyComponent : BodyComponent
    {
        private TransformComponent _transformComponent;

        [SerializeField]
        [InspectorName("Size")]
        private Vector3 _size = Vector3.One;

        internal override void Invalidate(string fieldName)
        {
            base.Invalidate(fieldName);

            switch (fieldName)
            {
                case nameof(_size):
                    PhysicsApi.SetBoxShape(BodyId, _size / 2);
                    break;
            }
        }

        protected override uint CreateBody(Vector3 position, Quaternion rotation, MotionType motionType, CollisionLayer collisionLayer)
        {
            Vector3 halfExtent = _size / 2;
            return PhysicsApi.CreateBoxBody(halfExtent, position, rotation, motionType, collisionLayer);
        }

        protected override void RenderCollider()
        {
            Gizmos.DrawCube(
                _transformComponent.Position,
                _transformComponent.Rotation,
                Vector3.One,
                _size,
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

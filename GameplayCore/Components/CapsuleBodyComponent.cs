using GameplayCore.Editor;
using GameplayCore.EngineApi;
using GameplayCore.Physics;
using GameplayCore.Serialization;
using GameplayCore.Mathematics;

namespace GameplayCore.Components
{
    internal class CapsuleBodyComponent : BodyComponent
    {
        private TransformComponent _transformComponent;

        [SerializeField]
        [InspectorName("Height")]
        private float _height = 1.0f;

        [SerializeField]
        [InspectorName("Radius")]
        private float _radius = 1.0f;

        internal override void Invalidate(string fieldName)
        {
            base.Invalidate(fieldName);

            switch (fieldName)
            {
                case nameof(_height):
                case nameof(_radius):
                    PhysicsApi.SetCapsuleShape(BodyId, _height / 2, _radius);
                    break;
            }
        }

        protected override uint CreateBody(Vector3 position, Quaternion rotation, MotionType motionType, CollisionLayer collisionLayer)
        {
             return PhysicsApi.CreateCapsuleBody(_height, _radius, position, rotation, motionType, collisionLayer);
        }

        protected override void RenderCollider()
        {
            Gizmos.DrawCapsule(
                _transformComponent.Position,
                _transformComponent.Rotation,
                Vector3.One,
                _height,
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

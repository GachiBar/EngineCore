using GameplayCore.EngineApi;
using GameplayCore.Mathematics;
using GameplayCore.Physics;

namespace GameplayCore.Components
{
    public abstract class CollisionComponent : Component
    {
        private TransformComponent _transformComponent;
        private RigidbodyComponent _rigidbodyComponent;

        protected uint BodyId => _rigidbodyComponent.BodyId;

        public override void Render()
        {
            if (_transformComponent != null)
            {
                RenderCollider(
                    _transformComponent.Position, 
                    _transformComponent.Rotation, 
                    Vector3.One, 
                    new Vector3(0.0f, 255.0f, 0.0f));
            }
        }

        protected override void OnAttach(GameObject gameObject)
        {            
            _rigidbodyComponent = GameObject.GetComponent<RigidbodyComponent>();
            _transformComponent = GameObject.GetComponent<TransformComponent>();

            if (_rigidbodyComponent != null && _transformComponent != null)
            {
                CreateShape();
            }

            gameObject.ComponentAdded += OnComponentAdded;
            gameObject.ComponentRemoved += OnComponentRemoved;
        }

        protected override void OnDetach(GameObject gameObject)
        {
            if (_rigidbodyComponent != null)
            {
                DestroyShape();
            }

            gameObject.ComponentAdded -= OnComponentAdded;
            gameObject.ComponentRemoved -= OnComponentRemoved;
        }

        protected abstract void SetShape();

        protected abstract void RenderCollider(Vector3 position, Quaternion rotation, Vector3 scale, Vector3 color);

        private void OnComponentAdded(GameObject gameObject, Component component)
        {
            if (component is RigidbodyComponent rigidbodyComponent)
            {
                _rigidbodyComponent = rigidbodyComponent;
                CreateShape();
            }
            if (component is TransformComponent transformComponent)
            {
                _transformComponent = transformComponent;

                if (_rigidbodyComponent != null)
                {
                    PhysicsApi.SetCollisionLayer(BodyId, CollisionLayer.Moving);
                }
            }
        }

        private void OnComponentRemoved(GameObject gameObject, Component component)
        {
            if (component is RigidbodyComponent)
            {
                _rigidbodyComponent = null;
            }
            if (component is TransformComponent)
            {
                _transformComponent = null;
                PhysicsApi.SetCollisionLayer(BodyId, CollisionLayer.NoCollision);
            }
        }

        private void CreateShape()
        {
            SetShape();
            var collisionLayer = _transformComponent == null ? CollisionLayer.NoCollision : CollisionLayer.Moving;
            PhysicsApi.SetCollisionLayer(BodyId, collisionLayer);
            PhysicsApi.SetActive(BodyId, true);
        }

        private void DestroyShape()
        {
            PhysicsApi.SetEmptyShape(BodyId);
            PhysicsApi.SetCollisionLayer(BodyId, CollisionLayer.NoCollision);
            PhysicsApi.SetActive(_rigidbodyComponent.BodyId, true);
        }
    }
}

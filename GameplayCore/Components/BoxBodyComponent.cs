using GameplayCore.EngineApi;
using GameplayCore.Mathematics;
using GameplayCore.Physics;

namespace GameplayCore.Components
{
    public class BoxBodyComponent : Component
    {
        private TransformComponent _transformComponent;
        private bool _isInitialized;
        private uint _bodyId;

        public bool IsStatic;

        public override void Initialize()
        {
            if (_transformComponent != null)
            {
                Vector3 halfExtent = _transformComponent.Scale / 2;
                Vector3 position = _transformComponent.Position;
                Quaternion rotation = _transformComponent.Rotation;
                MotionType motionType = IsStatic ? MotionType.Static : MotionType.Dynamic;
                _bodyId = PhysicsApi.CreateBoxBody(halfExtent, position, rotation, motionType, CollisionLayer.Moving);
                _isInitialized = true;
            }
        }

        public override void FixedUpdate()
        {
            if (_isInitialized && _transformComponent != null)
            {
                Vector3 position = Vector3.Zero;
                Quaternion rotation = Quaternion.Identity;
                PhysicsApi.GetBodyPositionAndRotation(_bodyId, ref position, ref rotation);
                _transformComponent.Position = position;
                _transformComponent.Rotation = rotation;
            }
        }

        public override void Terminate()
        {
            if (_isInitialized)
            {
                PhysicsApi.DestroyBody(_bodyId);
            }
        }

        protected override void OnAttach(GameObject gameObject)
        {
            _transformComponent = GameObject.GetComponent<TransformComponent>();

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

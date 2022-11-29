using GameplayCore.Editor;
using GameplayCore.EngineApi;
using GameplayCore.Mathematics;
using GameplayCore.Physics;
using GameplayCore.Serialization;

namespace GameplayCore.Components
{
    public class BoxBodyComponent : Component
    {
        private TransformComponent _transformComponent;
        private bool _isInitialized;
        private uint _bodyId;

        Vector3 _position;
        Quaternion _rotation;

        [SerializeField]
        [InspectorName("IsStatic")]
        private bool _isStatic = false;
        [SerializeField]
        [InspectorName("IsActive")]
        private bool _isActive = false;
        [SerializeField]
        [InspectorName("Size")]
        private Vector3 _size = Vector3.One;

        public bool IsStatic
        {
            get => _isStatic;
            set 
            {
                _isStatic = value;
                MotionType motionType = IsStatic ? MotionType.Static : MotionType.Dynamic;
                PhysicsApi.SetMotionType(_bodyId, motionType);
            }
        }

        public bool IsActive
        {
            get => _isActive;
            set
            {
                _isActive = value;
                PhysicsApi.SetActive(_bodyId, _isActive);
            }
        }

        public override void Initialize()
        {
            if (_transformComponent != null)
            {
                Vector3 halfExtent = _size / 2;
                _position = _transformComponent.Position;
                _rotation = _transformComponent.Rotation;
                MotionType motionType = IsStatic ? MotionType.Static : MotionType.Dynamic;
                _bodyId = PhysicsApi.CreateBoxBody(halfExtent, _position, _rotation, motionType, CollisionLayer.Moving);
                _isInitialized = true;
            }
        }

        public override void FixedUpdate()
        {
            if (_isInitialized && _transformComponent != null)
            {               
                if (_position != _transformComponent.Position || 
                    _rotation != _transformComponent.Rotation)
                {
                    PhysicsApi.SetBodyPositionAndRotation(_bodyId, _transformComponent.Position, _transformComponent.Rotation);
                    PhysicsApi.SetActive(_bodyId, _isActive);
                }

                PhysicsApi.GetBodyPositionAndRotation(_bodyId, ref _position, ref _rotation);
                _transformComponent.Position = _position;
                _transformComponent.Rotation = _rotation;
            }
        }

        public override void Render()
        {
            if (_transformComponent != null)
            {
                Gizmos.DrawCube(
                    _transformComponent.Position, 
                    _transformComponent.Rotation, 
                    Vector3.One,
                    _size, 
                    new Vector3(0.0f, 255.0f, 0.0f));
            }
        }

        public override void Terminate()
        {
            if (_isInitialized)
            {
                PhysicsApi.DestroyBody(_bodyId);
            }
        }

        public void AddForce(Vector3 force)
        {
            if (_isInitialized && _transformComponent != null)
            {
                PhysicsApi.AddForce(_bodyId, force);
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

        internal override void Invalidate(string fieldName)
        {
            switch (fieldName)
            {
                case nameof(_isStatic):
                    PhysicsApi.SetMotionType(_bodyId, _isStatic ? MotionType.Static : MotionType.Dynamic);
                    _isActive = PhysicsApi.IsActive(_bodyId);
                    break;
                case nameof(_isActive):
                    PhysicsApi.SetActive(_bodyId, _isActive);
                    break;
                case nameof(_size):
                    PhysicsApi.SetBoxShape(_bodyId, _size / 2);
                    break;
            }
        }
    }
}

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
        [InspectorName("Size")]
        private Vector3 _size = Vector3.One;

        public bool IsStatic
        {
            get => _isStatic;
            set 
            {
                _isStatic = value;

                if (_isStatic)
                {
                    PhysicsApi.SetMotionType(_bodyId, MotionType.Static);
                }
                else
                {
                    PhysicsApi.SetMotionType(_bodyId, MotionType.Dynamic);
                    PhysicsApi.SetActive(_bodyId, true);
                }
            }
        }

        public override void Initialize()
        {
            if (_transformComponent != null)
            {
                Vector3 halfExtent = _size / 2;
                _position = _transformComponent.Position;
                _rotation = _transformComponent.Rotation;

                if (IsStatic)
                {
                    _bodyId = PhysicsApi.CreateBoxBody(halfExtent, _position, _rotation, MotionType.Static, CollisionLayer.Moving);
                }
                else
                {
                    _bodyId = PhysicsApi.CreateBoxBody(halfExtent, _position, _rotation, MotionType.Dynamic, CollisionLayer.Moving);
                    PhysicsApi.SetActive(_bodyId, true);
                }

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
                    PhysicsApi.SetActive(_bodyId, true);
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
                    if (_isStatic)
                    {
                        PhysicsApi.SetMotionType(_bodyId, MotionType.Static);
                    }
                    else
                    {
                        PhysicsApi.SetMotionType(_bodyId, MotionType.Dynamic);
                        PhysicsApi.SetActive(_bodyId, true);
                    }
                    break;
                case nameof(_size):
                    PhysicsApi.SetBoxShape(_bodyId, _size / 2);
                    break;
            }
        }
    }
}

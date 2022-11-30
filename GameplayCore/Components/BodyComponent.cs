using GameplayCore.Editor;
using GameplayCore.EngineApi;
using GameplayCore.Physics;
using GameplayCore.Mathematics;
using GameplayCore.Serialization;

namespace GameplayCore.Components
{
    public abstract class BodyComponent : Component
    {
        private TransformComponent _transformComponent;
        private bool _isInitialized;
        private uint _bodyId;

        private Vector3 _position = Vector3.Zero;
        private Quaternion _rotation = Quaternion.Identity;

        [SerializeField]
        [InspectorName("IsStatic")]
        private bool _isStatic = false;

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

        protected uint BodyId => _bodyId;

        protected Vector3 Position 
        { 
            get => _position; 
            private set => _position = value; 
        }

        protected Quaternion Rotation 
        { 
            get => _rotation; 
            private set => _rotation = value; 
        }

        public override void Initialize()
        {
            if (_transformComponent != null)
            {
                _position = _transformComponent.Position;
                _rotation = _transformComponent.Rotation;

                if (IsStatic)
                {
                    _bodyId = CreateBody(_position, _rotation, MotionType.Static, CollisionLayer.Moving);
                }
                else
                {
                    _bodyId = CreateBody(_position, _rotation, MotionType.Dynamic, CollisionLayer.Moving);
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
                _transformComponent.Position = Position;
                _transformComponent.Rotation = Rotation;
            }
        }

        public override void Render()
        {
            if (_transformComponent != null)
            {
                RenderCollider();
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

        protected abstract uint CreateBody(Vector3 position, Quaternion rotation, MotionType motionType, CollisionLayer collisionLayer);

        protected abstract void RenderCollider();

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
            }
        }
    }
}

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
                    PhysicsApi.SetMotionType(BodyId, MotionType.Static);
                }
                else
                {
                    PhysicsApi.SetMotionType(BodyId, MotionType.Dynamic);
                    PhysicsApi.SetActive(BodyId, true);
                }
            }
        }

        protected uint BodyId
        {
            get => _bodyId;
            private set => _bodyId = value;
        }

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
                Position = _transformComponent.Position;
                Rotation = _transformComponent.Rotation;

                if (IsStatic)
                {
                    BodyId = CreateBody(_position, _rotation, MotionType.Static, CollisionLayer.Moving);
                }
                else
                {
                    BodyId = CreateBody(_position, _rotation, MotionType.Dynamic, CollisionLayer.Moving);
                    PhysicsApi.SetActive(BodyId, true);
                }

                _isInitialized = true;
            }
        }

        public override void FixedUpdate()
        {
            if (_isInitialized && _transformComponent != null)
            {
                if (Position != _transformComponent.Position ||
                    Rotation != _transformComponent.Rotation)
                {
                    PhysicsApi.SetBodyPosition(BodyId, _transformComponent.Position);
                    PhysicsApi.SetBodyRotation(BodyId, _transformComponent.Rotation);
                    PhysicsApi.SetActive(BodyId, true);
                }

                Position = PhysicsApi.GetBodyPosition(BodyId);
                Rotation = PhysicsApi.GetBodyRoation(BodyId);
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
                PhysicsApi.DestroyBody(BodyId);
            }
        }

        public void AddForce(Vector3 force)
        {
            if (_isInitialized && _transformComponent != null)
            {
                PhysicsApi.AddForce(BodyId, force);
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
                        PhysicsApi.SetMotionType(BodyId, MotionType.Static);
                    }
                    else
                    {
                        PhysicsApi.SetMotionType(BodyId, MotionType.Dynamic);
                        PhysicsApi.SetActive(BodyId, true);
                    }
                    break;
            }
        }
    }
}

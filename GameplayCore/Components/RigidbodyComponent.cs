using GameplayCore.Editor;
using GameplayCore.EngineApi;
using GameplayCore.Mathematics;
using GameplayCore.Physics;
using GameplayCore.Serialization;

namespace GameplayCore.Components
{
    public class RigidbodyComponent : Component
    {
        private TransformComponent _transformComponent = null;
        private uint _bodyId = 0;

        private Vector3 _position = Vector3.Zero;
        private Quaternion _rotation = Quaternion.Identity;

        [SerializeField]
        [InspectorName("IsKinematic")]
        private bool _isKinematic = false;

        [SerializeField]
        [InspectorName("Mass")]
        private float _mass = 1.0f;
        
        //[SerializeField]
        //[InspectorName("Friction (not implemented)")]
        //private float _friction = 0.0f;

        //[SerializeField]
        //[InspectorName("GravityFactor (not implemented)")]
        //private float _gravityFactor = 0.0f;

        [SerializeField]
        [InspectorName("FreezeRotation")]
        public bool _freezeRotation = false;

        public uint BodyId => _bodyId;

        public bool IsKinematic
        {
            get => _isKinematic;
            set
            {
                _isKinematic = value;
                PhysicsApi.SetMotionType(BodyId, _isKinematic ? MotionType.Kinematic : MotionType.Dynamic);
                PhysicsApi.SetActive(BodyId, _isKinematic);
            }
        }

        public float Mass
        {
            get => _mass;
            set
            {
                _mass = value;
                PhysicsApi.SetMass(BodyId, _mass);

                if (_freezeRotation)
                {
                    PhysicsApi.FreezeRotation(BodyId);
                }
            }
        }

        public bool FreezeRotation
        {
            get => _freezeRotation;
            set
            {
                _freezeRotation = value;

                if (_freezeRotation)
                {
                    PhysicsApi.FreezeRotation(BodyId);
                }
                else
                {
                    PhysicsApi.SetMass(BodyId, _mass);
                }
            }
        }

        public Vector3 Velocity
        {
            get => PhysicsApi.GetLinearVelocity(BodyId);
            set => PhysicsApi.SetLinearVelocity(BodyId, value);
        }

        public override void FixedUpdate()
        {
            if (_transformComponent != null)
            {
                if (_position != _transformComponent.Position ||
                    _rotation != _transformComponent.Rotation)
                {
                    PhysicsApi.SetBodyPosition(BodyId, _transformComponent.Position);
                    PhysicsApi.SetBodyRotation(BodyId, _transformComponent.Rotation);
                    PhysicsApi.SetActive(BodyId, true);
                }

                _position = PhysicsApi.GetBodyPosition(BodyId);
                _rotation = PhysicsApi.GetBodyRotation(BodyId);
                _transformComponent.Position = _position;
                _transformComponent.Rotation = _rotation;
            }
        }

        public void AddForce(Vector3 force)
        {
            if (_transformComponent != null)
            {
                PhysicsApi.AddForce(BodyId, force);
            }
        }

        public void AddImpulse(Vector3 impulse)
        {
            if (_transformComponent != null)
            {
                PhysicsApi.AddImpulse(BodyId, impulse);
            }
        }

        internal override void Invalidate(string fieldName)
        {
            switch (fieldName)
            {
                case nameof(_isKinematic):
                    IsKinematic = _isKinematic;
                    break;
                case nameof(_freezeRotation):
                    FreezeRotation = _freezeRotation;
                    break;
            }
        }

        protected override void OnAttach(GameObject gameObject)
        {
            _transformComponent = GameObject.GetComponent<TransformComponent>();
            _bodyId = PhysicsApi.CreateBody(
                _position, 
                _rotation,
                _isKinematic ? MotionType.Kinematic : MotionType.Dynamic);
            PhysicsApi.SetMass(_bodyId, _mass);

            if (_freezeRotation)
            {
                PhysicsApi.FreezeRotation(_bodyId);
            }

            gameObject.ComponentAdded += OnComponentAdded;
            gameObject.ComponentRemoved += OnComponentRemoved;
        }

        protected override void OnDetach(GameObject gameObject)
        {
            PhysicsApi.DestroyBody(_bodyId);

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

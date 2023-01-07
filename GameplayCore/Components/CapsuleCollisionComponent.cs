using GameplayCore.Editor;
using GameplayCore.EngineApi;
using GameplayCore.Serialization;
using GameplayCore.Mathematics;

namespace GameplayCore.Components
{
    internal class CapsuleCollisionComponent : CollisionComponent
    {
        [SerializeField]
        [InspectorName("Height")]
        private float _height = 1.0f;

        [SerializeField]
        [InspectorName("Radius")]
        private float _radius = 1.0f;

        public float Height
        {
            get => _height;
            set
            {
                _height = value;
                SetShape();
                PhysicsApi.SetActive(BodyId, true);
            }
        }

        public float Radius
        {
            get => _radius;
            set
            {
                _radius = value;
                SetShape();
                PhysicsApi.SetActive(BodyId, true);
            }
        }

        internal override void Invalidate(string fieldName)
        {
            switch (fieldName)
            {
                case nameof(_height):
                    Height = _height;
                    break;
                case nameof(_radius):
                    Radius = _radius;
                    break;
            }
        }

        protected override void SetShape()
        {
            float halfHeight = _height / 2;
            PhysicsApi.SetCapsuleShape(BodyId, halfHeight, _radius);
        }

        protected override void RenderCollider(Vector3 position, Quaternion rotation, Vector3 scale, Vector3 color)
        {
            Gizmos.DrawCapsule(position, rotation, scale, _height, _radius, color);
        }
    }
}

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

        internal override void Invalidate(string fieldName)
        {
            switch (fieldName)
            {
                case nameof(_height):
                case nameof(_radius):
                    SetShape();
                    PhysicsApi.SetActive(BodyId, true);
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

using GameplayCore.Editor;
using GameplayCore.EngineApi;
using GameplayCore.Mathematics;
using GameplayCore.Serialization;

namespace GameplayCore.Components
{
    public class BoxCollisionComponent : CollisionComponent
    {
        [SerializeField]
        [InspectorName("Size")]
        private Vector3 _size = Vector3.One;

        internal override void Invalidate(string fieldName)
        {
            base.Invalidate(fieldName);

            switch (fieldName)
            {
                case nameof(_size):
                    SetShape();
                    PhysicsApi.SetActive(BodyId, true);
                    break;
            }
        }

        protected override void SetShape()
        {
            Vector3 halfExtent = _size;
            PhysicsApi.SetBoxShape(BodyId, halfExtent);            
        }

        protected override void RenderCollider(Vector3 position, Quaternion rotation, Vector3 scale, Vector3 color)
        {
            Gizmos.DrawCube(position, rotation, scale, _size * 2, color);            
        }
    }
}

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

        public Vector3 Size
        {
            get => _size;
            set
            {
                _size = value;
                SetShape();
                PhysicsApi.SetActive(BodyId, true);
            }
        } 

        protected override void SetShape()
        {
            Vector3 halfExtent = _size / 2;
            PhysicsApi.SetBoxShape(BodyId, halfExtent);            
        }

        protected override void RenderCollider(Vector3 position, Quaternion rotation, Vector3 scale, Vector3 color)
        {
            Gizmos.DrawCube(position, rotation, scale, _size, color);            
        }
    }
}

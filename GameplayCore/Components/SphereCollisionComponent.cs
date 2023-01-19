using GameplayCore.Editor;
using GameplayCore.EngineApi;
using GameplayCore.Mathematics;
using GameplayCore.Serialization;

namespace GameplayCore.Components
{
    public class SphereCollisionComponent : CollisionComponent
    {       
        [SerializeField]
        [InspectorName("Radius")]
        private float _radius = 1.0f;

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

        protected override void SetShape()
        {
            PhysicsApi.SetSphereShape(BodyId, _radius);
        }

        protected override void RenderCollider(Vector3 position, Quaternion rotation, Vector3 scale, Vector3 color)
        {
            Gizmos.DrawSphere(position, rotation, scale, _radius, color);
        }
    }
}

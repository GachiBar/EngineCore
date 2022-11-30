using GameplayCore.Editor;
using GameplayCore.EngineApi;
using GameplayCore.Mathematics;
using GameplayCore.Physics;
using GameplayCore.Serialization;

namespace GameplayCore.Components
{
    public class SphereBodyComponent : BodyComponent
    {
        [SerializeField]
        [InspectorName("Radius")]
        private float _radius = 1.0f;

        internal override void Invalidate(string fieldName)
        {
            base.Invalidate(fieldName);

            switch (fieldName)
            {
                case nameof(_radius):
                    PhysicsApi.SetSphereShape(BodyId, _radius);
                    break;
            }
        }

        protected override uint CreateBody(Vector3 position, Quaternion rotation, MotionType motionType, CollisionLayer collisionLayer)
        {
            return PhysicsApi.CreateSphereBody(_radius, position, rotation, motionType, collisionLayer);
        }

        protected override void RenderCollider()
        {
            Gizmos.DrawSphere(Position, _radius, new Vector3(0.0f, 255.0f, 0.0f));
            //Gizmos.DrawCube(
            //    Position,
            //    Rotation,
            //    Vector3.One,
            //    new Vector3(_radius, _radius, _radius),
            //    new Vector3(0.0f, 255.0f, 0.0f));
        }
    }
}

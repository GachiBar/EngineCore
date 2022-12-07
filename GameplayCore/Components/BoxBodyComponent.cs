using GameplayCore.Editor;
using GameplayCore.EngineApi;
using GameplayCore.Mathematics;
using GameplayCore.Physics;
using GameplayCore.Serialization;

namespace GameplayCore.Components
{
    public class BoxBodyComponent : BodyComponent
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
                    PhysicsApi.SetBoxShape(BodyId, _size / 2);
                    break;
            }
        }

        protected override uint CreateBody(Vector3 position, Quaternion rotation, MotionType motionType, CollisionLayer collisionLayer)
        {
            Vector3 halfExtent = _size / 2;
            return PhysicsApi.CreateBoxBody(halfExtent, position, rotation, motionType, collisionLayer);
        }

        protected override void RenderCollider()
        {
            Gizmos.DrawCube(
                Position,
                Rotation,
                Vector3.One,
                _size,
                new Vector3(0.0f, 255.0f, 0.0f));            
        }
    }
}

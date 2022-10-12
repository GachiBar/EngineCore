using GameplayCore.Mathematics;

namespace GameplayCore.Components
{
    public class TransformComponent : Component
    {
        public Vector3 LocalEuler;

        public Vector3 LocalScale { get; set; }

        public Vector3 LocalPosition { get; set; }
    }
}

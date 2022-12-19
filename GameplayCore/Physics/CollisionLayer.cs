using System;

namespace GameplayCore.Physics
{
    [Flags]
    public enum CollisionLayer : byte
    {
        NoCollision,
        NonMoving,
        Moving,
    }
}

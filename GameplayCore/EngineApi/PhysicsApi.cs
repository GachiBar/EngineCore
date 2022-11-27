using GameplayCore.Mathematics;
using GameplayCore.Physics;
using System.Runtime.CompilerServices;

namespace GameplayCore.EngineApi
{
    internal static class PhysicsApi
    {
        private static unsafe void* PhysicsSystem { get; set; }

        public static uint CreateSphereBody(
            float radius,
            Vector3 position,
            Quaternion rotation,
            MotionType motionType,
            CollisionLayer collisionLayer)
        {
            unsafe
            {
                return Internal_CreateSphereBody(PhysicsSystem, radius, new Vector4(position, position.Z), rotation, motionType, collisionLayer);
            }            
        }

        public static uint CreateBoxBody(
            Vector3 halfExtent,
            Vector3 position,
            Quaternion rotation,
            MotionType motionType,
            CollisionLayer collisionLayer)
        {
            unsafe
            {
                return Internal_CreateBoxBody(PhysicsSystem, new Vector4(halfExtent, halfExtent.Z), new Vector4(position, position.Z), rotation, motionType, collisionLayer);
            }
        }

        public static void DestroyBody(uint bodyId)
        {
            unsafe
            {
                Internal_DestroyBody(PhysicsSystem, bodyId);
            }
        }

        public static void SetActive(uint bodyId, bool isActive)
        {
            unsafe
            {
                Internal_SetActive(PhysicsSystem, bodyId, isActive);
            }
        }

        public static void GetBodyPositionAndRotation(
            uint bodyId,
            ref Vector3 position,
            ref Quaternion rotation)
        {
            unsafe
            {
                var outPosition = new Vector4(position, position.Z);
                Internal_GetBodyPositionAndRotation(PhysicsSystem, bodyId, ref outPosition, ref rotation);
                position = new Vector3(outPosition.X, outPosition.Y, outPosition.Z);
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe uint Internal_CreateSphereBody(
            void* physicsSystem, 
            float radius, 
            Vector4 position, 
            Quaternion rotation,
            MotionType motionType, 
            CollisionLayer collisionLayer);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe uint Internal_CreateBoxBody(
            void* physicsSystem,
            Vector4 halfExtent,
            Vector4 position,
            Quaternion rotation,
            MotionType motionType,
            CollisionLayer collisionLayer);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_DestroyBody(void* physicsSystem, uint bodyId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_SetActive(void* physicsSystem, uint bodyId, bool isActive);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_GetBodyPositionAndRotation(
            void* physicsSystem, 
            uint bodyId, 
            ref Vector4 position,
            ref Quaternion rotation);
    }
}

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
                var inPosition = new Vector4(position, position.Z);
                return Internal_CreateSphereBody(PhysicsSystem, radius, inPosition, rotation, motionType, collisionLayer);
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
                var inHalfExtent = new Vector4(halfExtent, halfExtent.Z);
                var inPosition = new Vector4(position, position.Z);
                return Internal_CreateBoxBody(PhysicsSystem, inHalfExtent, inPosition, rotation, motionType, collisionLayer);
            }
        }

        public static void DestroyBody(uint bodyId)
        {
            unsafe
            {
                Internal_DestroyBody(PhysicsSystem, bodyId);
            }
        }

        public static void SetSphereShape(uint bodyId, float radius)
        {
            unsafe
            {
                Internal_SetSphereShape(PhysicsSystem, bodyId, radius);
            }
        }

        public static void SetBoxShape(uint bodyId, Vector3 halfExtent)
        {
            unsafe
            {
                var inHalfExtent = new Vector4(halfExtent, halfExtent.Z);
                Internal_SetBoxShape(PhysicsSystem, bodyId, inHalfExtent);
            }
        }

        public static void SetMotionType(uint bodyId, MotionType motionType)
        {
            unsafe
            {
                Internal_SetMotionType(PhysicsSystem, bodyId, motionType);
            }
        }

        public static void SetActive(uint bodyId, bool isActive)
        {
            unsafe
            {
                Internal_SetActive(PhysicsSystem, bodyId, isActive);
            }
        }

        public static bool IsActive(uint bodyId)
        {
            unsafe
            {
                return Internal_IsActive(PhysicsSystem, bodyId);
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

        public static void SetBodyPositionAndRotation(
            uint bodyId,
            Vector3 position,
            Quaternion rotation)
        {
            unsafe
            {
                var inPosition = new Vector4(position, position.Z);
                Internal_SetBodyPositionAndRotation(PhysicsSystem, bodyId, inPosition, rotation);
            }
        }

        public static void AddForce(uint bodyId, Vector3 force)
        {
            unsafe
            {
                var inForce = new Vector4(force, force.Z);
                Internal_AddForce(PhysicsSystem, bodyId, inForce);
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
        extern private static unsafe void Internal_SetSphereShape(void* physicsSystem, uint bodyId, float radius);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_SetBoxShape(void* physicsSystem, uint bodyId, Vector4 halfExtent);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_SetMotionType(void* physicsSystem, uint bodyId, MotionType motionType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_SetActive(void* physicsSystem, uint bodyId, bool isActive);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe bool Internal_IsActive(void* physicsSystem, uint bodyId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_GetBodyPositionAndRotation(
            void* physicsSystem, 
            uint bodyId, 
            ref Vector4 position,
            ref Quaternion rotation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_SetBodyPositionAndRotation(
            void* physicsSystem,
            uint bodyId,
            Vector4 position,
            Quaternion rotation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_AddForce(void* physicsSystem, uint bodyId, Vector4 force);
    }
}

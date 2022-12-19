using GameplayCore.Mathematics;
using GameplayCore.Physics;
using System.Runtime.CompilerServices;

namespace GameplayCore.EngineApi
{
    internal static class PhysicsApi
    {
        private static unsafe void* PhysicsSystem { get; set; }

        public static uint CreateBody(
            Vector3 position,
            Quaternion rotation,
            MotionType motionType)
        {
            unsafe
            {
                var inPosition = new Vector4(position, position.Z);
                return Internal_CreateBody(PhysicsSystem, inPosition, rotation, motionType);
            }
        }

        public static void DestroyBody(uint bodyId)
        {
            unsafe
            {
                Internal_DestroyBody(PhysicsSystem, bodyId);
            }
        }

        public static void SetCollisionLayer(uint bodyId, CollisionLayer collisionLayer)
        {
            unsafe
            {
                Internal_SetCollisionLayer(PhysicsSystem, bodyId, collisionLayer);
            }
        }

        public static void SetEmptyShape(uint bodyId) 
        {
            unsafe
            {
                Internal_SetEmptyShape(PhysicsSystem, bodyId);
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

        public static void SetCapsuleShape(uint bodyId, float halfHeight, float radius)
        {
            unsafe
            {
                Internal_SetCapsuleShape(PhysicsSystem, bodyId, halfHeight, radius);
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

        public static Vector3 GetBodyPosition(uint bodyId)
        {
            unsafe
            {
                var position = Internal_GetBodyPosition(PhysicsSystem, bodyId);
                return new Vector3(position.X, position.Y, position.Z);
            }
        }

        public static void SetBodyPosition(uint bodyId, Vector3 position)
        {
            unsafe
            {
                Internal_SetBodyPosition(PhysicsSystem, bodyId, new Vector4(position, position.Z));
            }
        }

        public static Quaternion GetBodyRotation(uint bodyId)
        {
            unsafe
            {
                return Internal_GetBodyRotation(PhysicsSystem, bodyId);
            }
        }

        public static void SetBodyRotation(uint bodyId, Quaternion rotation)
        {
            unsafe
            {
                Internal_SetBodyRotation(PhysicsSystem, bodyId, rotation);
            }
        }

        public static void FreezeRotation(uint bodyId)
        {
            unsafe
            {
                Internal_FreezeRotation(PhysicsSystem, bodyId);
            }
        }

        public static void SetMass(uint bodyId, float mass)
        {
            unsafe
            {
                Internal_SetMass(PhysicsSystem, bodyId, mass);
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

        public static void AddImpulse(uint bodyId, Vector3 impulse)
        {
            unsafe
            {
                var inImpulse = new Vector4(impulse, impulse.Z);
                Internal_AddImpulse(PhysicsSystem, bodyId, inImpulse);
            }
        }

        public static Vector3 GetLinearVelocity(uint bodyId)
        {
            unsafe
            {
                var velocity = Internal_GetLinearVelocity(PhysicsSystem, bodyId);
                return new Vector3(velocity.X, velocity.Y, velocity.Z);
            }
        }

        public static void SetLinearVelocity(uint bodyId, Vector3 velocity)
        {
            unsafe
            {
                var inVelocity = new Vector4(velocity, velocity.Z);
                Internal_SetLinearVelocity(PhysicsSystem, bodyId, inVelocity);
            }
        }

        public static bool CastRay(Vector3 origin, Vector3 direction, out uint bodyId)
        {
            unsafe
            {
                var inOrigin = new Vector4(origin, origin.Z);
                var inDirection = new Vector4(direction, direction.Z);
                return Internal_CastRay(PhysicsSystem, inOrigin, inDirection, out bodyId);
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe uint Internal_CreateBody(
            void* physicsSystem,
            Vector4 position,
            Quaternion rotation,
            MotionType motionType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_DestroyBody(void* physicsSystem, uint bodyId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_SetCollisionLayer(void* physicsSystem, uint bodyId, CollisionLayer collisionLayer);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_SetEmptyShape(void* physicsSystem, uint bodyId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_SetSphereShape(void* physicsSystem, uint bodyId, float radius);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_SetBoxShape(void* physicsSystem, uint bodyId, Vector4 halfExtent);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_SetCapsuleShape(void* physicsSystem, uint bodyId, float halfHeight, float radius);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_SetMotionType(void* physicsSystem, uint bodyId, MotionType motionType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_SetActive(void* physicsSystem, uint bodyId, bool isActive);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe bool Internal_IsActive(void* physicsSystem, uint bodyId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe Vector4 Internal_GetBodyPosition(void* physicsSystem, uint bodyId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_SetBodyPosition(void* physicsSystem, uint bodyId, Vector4 position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe Quaternion Internal_GetBodyRotation(void* physicsSystem, uint bodyId);        

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_SetBodyRotation(void* physicsSystem, uint bodyId, Quaternion rotation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_FreezeRotation(void* physicsSystem, uint bodyId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_SetMass(void* physicsSystem, uint bodyId, float mass);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_AddForce(void* physicsSystem, uint bodyId, Vector4 force);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_AddImpulse(void* physicsSystem, uint bodyId, Vector4 impulse);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe Vector4 Internal_GetLinearVelocity(void* physicsSystem, uint bodyId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_SetLinearVelocity(void* physicsSystem, uint bodyId, Vector4 velocity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe bool Internal_CastRay(void* physicsSystem, Vector4 origin, Vector4 direction, out uint bodyId);
    }
}

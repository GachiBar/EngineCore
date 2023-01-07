using GameplayCore.Components;
using GameplayCore.Mathematics;
using GameplayCore.EngineApi;

namespace GameplayCore
{
    public static class Physics
    {
        public static bool CastRay(Vector3 origin, Vector3 direction, out RigidbodyComponent rigidbody)
        {
            var result = PhysicsApi.CastRay(origin, direction, out var bodyId);
            rigidbody = result ? PhysicsApi.GetRigidbodyById(bodyId) : null;
            return result;
        }
    }
}

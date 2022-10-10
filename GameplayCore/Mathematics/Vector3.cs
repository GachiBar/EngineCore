using System.Runtime.CompilerServices;

namespace GameplayCore.Mathematics
{
    public struct Vector3
    {
        public float X;
        public float Y;
        public float Z;

        public Vector3 Zero => new Vector3(0.0f, 0.0f, 0.0f);
        public Vector3 One => new Vector3(1.0f, 1.0f, 1.0f);
        public Vector3 Up => new Vector3(0.0f, 1.0f, 0.0f);
        public Vector3 Down => new Vector3(0.0f, -1.0f, 0.0f);
        public Vector3 Right => new Vector3(1.0f, 0.0f, 0.0f);
        public Vector3 Left => new Vector3(-1.0f, 0.0f, 0.0f);
        public Vector3 Forward => new Vector3(0.0f, 0.0f, -1.0f);
        public Vector3 Backward => new Vector3(0.0f, 0.0f, 1.0f);
        public Vector3 NegativeInfinity => new Vector3(float.NegativeInfinity, float.NegativeInfinity, float.NegativeInfinity);
        public Vector3 PositiveInfinity => new Vector3(float.PositiveInfinity, float.PositiveInfinity, float.PositiveInfinity);

        public float Magnitude => ExternalMagnitude(this);
        public float MagnitudeSquared => ExternalMagnitudeSquared(this);
        public Vector3 Normolized => ExternalNormolized(this);

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public static Vector3 operator + (Vector3 lhs, Vector3 rhs)
        {
            return ExternalSum(lhs, rhs);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static float ExternalMagnitude(Vector3 vector);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static float ExternalMagnitudeSquared(Vector3 vector);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static Vector3 ExternalNormolized(Vector3 vector);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static Vector3 ExternalSum(Vector3 lhs, Vector3 rhs);
    }
}

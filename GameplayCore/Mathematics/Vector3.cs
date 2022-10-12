using System;
using System.Runtime.InteropServices;
using System.Globalization;
using System.Runtime.CompilerServices;

namespace GameplayCore.Mathematics
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3 : IEquatable<Vector3>, IFormattable
    {
        public const float kEpsilon = 0.00001f;
        public const float kEpsilonNormalSqrt = 1e-15f;

        public float X;
        public float Y;
        public float Z;

        public static Vector3 Zero => new Vector3(0.0f, 0.0f, 0.0f);
        public static Vector3 One => new Vector3(1.0f, 1.0f, 1.0f);
        public static Vector3 Forward => new Vector3(0.0f, 0.0f, -1.0f);
        public static Vector3 Back => new Vector3(0.0f, 0.0f, 1.0f);
        public static Vector3 Up => new Vector3(0.0f, 1.0f, 0.0f);
        public static Vector3 Down => new Vector3(0.0f, -1.0f, 0.0f);
        public static Vector3 Left => new Vector3(-1.0f, 0.0f, 0.0f);
        public static Vector3 Right => new Vector3(1.0f, 0.0f, 0.0f);
        public static Vector3 PositiveInfinity => new Vector3(float.PositiveInfinity, float.PositiveInfinity, float.PositiveInfinity);
        public static Vector3 NegativeInfinity => new Vector3(float.NegativeInfinity, float.NegativeInfinity, float.NegativeInfinity);

        public Vector3 Normalized => Vector3.Normalize(this);
        public float Magnitude => (float)Math.Sqrt(X * X + Y * Y + Z * Z);
        public float MagnitudeSquared => X * X + Y * Y + Z * Z;

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public Vector3(float x, float y)
        {
            X = x;
            Y = y;
            Z = 0.0f;
        }

        public void Set(float newX, float newY, float newZ)
        {
            X = newX;
            Y = newY;
            Z = newZ;
        }

        public static Vector3 Lerp(Vector3 a, Vector3 b, float t)
        {
            t = MathF.Clamp01(t);
            return new Vector3(
                a.X + (b.X - a.X) * t,
                a.Y + (b.Y - a.Y) * t,
                a.Z + (b.Z - a.Z) * t);
        }

        public static Vector3 LerpUnclamped(Vector3 a, Vector3 b, float t)
        {
            return new Vector3(
                a.X + (b.X - a.X) * t,
                a.Y + (b.Y - a.Y) * t,
                a.Z + (b.Z - a.Z) * t);
        }

        public static Vector3 MoveTowards(Vector3 current, Vector3 target, float maxDistanceDelta)
        {
            // avoid vector ops because current scripting backends are terrible at inlining
            float toVector_x = target.X - current.X;
            float toVector_y = target.Y - current.Y;
            float toVector_z = target.Z - current.Z;

            float sqdist = toVector_x * toVector_x + toVector_y * toVector_y + toVector_z * toVector_z;

            if (sqdist == 0 || (maxDistanceDelta >= 0 && sqdist <= maxDistanceDelta * maxDistanceDelta))
            {
                return target;
            }

            var dist = (float)Math.Sqrt(sqdist);

            return new Vector3(
                current.X + toVector_x / dist * maxDistanceDelta,
                current.Y + toVector_y / dist * maxDistanceDelta,
                current.Z + toVector_z / dist * maxDistanceDelta);
        }

        //public static Vector3 SmoothDamp(Vector3 current, Vector3 target, ref Vector3 currentVelocity, float smoothTime, float maxSpeed)
        //{
        //    float deltaTime = Time.deltaTime;
        //    return SmoothDamp(current, target, ref currentVelocity, smoothTime, maxSpeed, deltaTime);
        //}

        //public static Vector3 SmoothDamp(Vector3 current, Vector3 target, ref Vector3 currentVelocity, float smoothTime)
        //{
        //    float deltaTime = Time.deltaTime;
        //    float maxSpeed = Mathf.Infinity;
        //    return SmoothDamp(current, target, ref currentVelocity, smoothTime, maxSpeed, deltaTime);
        //}

        public static Vector3 SmoothDamp(Vector3 current, Vector3 target, ref Vector3 currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
        {
            float output_x = 0.0f;
            float output_y = 0.0f;
            float output_z = 0.0f;

            // Based on Game Programming Gems 4 Chapter 1.10
            smoothTime = MathF.Max(0.0001f, smoothTime);
            float omega = 2.0f / smoothTime;

            float x = omega * deltaTime;
            float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);

            float change_x = current.X - target.X;
            float change_y = current.Y - target.Y;
            float change_z = current.Z - target.Z;
            Vector3 originalTo = target;

            // Clamp maximum speed
            float maxChange = maxSpeed * smoothTime;

            float maxChangeSq = maxChange * maxChange;
            float sqrmag = change_x * change_x + change_y * change_y + change_z * change_z;

            if (sqrmag > maxChangeSq)
            {
                var mag = (float)Math.Sqrt(sqrmag);
                change_x = change_x / mag * maxChange;
                change_y = change_y / mag * maxChange;
                change_z = change_z / mag * maxChange;
            }

            target.X = current.X - change_x;
            target.Y = current.Y - change_y;
            target.Z = current.Z - change_z;

            float temp_x = (currentVelocity.X + omega * change_x) * deltaTime;
            float temp_y = (currentVelocity.Y + omega * change_y) * deltaTime;
            float temp_z = (currentVelocity.Z + omega * change_z) * deltaTime;

            currentVelocity.X = (currentVelocity.X - omega * temp_x) * exp;
            currentVelocity.Y = (currentVelocity.Y - omega * temp_y) * exp;
            currentVelocity.Z = (currentVelocity.Z - omega * temp_z) * exp;

            output_x = target.X + (change_x + temp_x) * exp;
            output_y = target.Y + (change_y + temp_y) * exp;
            output_z = target.Z + (change_z + temp_z) * exp;

            // Prevent overshooting
            float origMinusCurrent_x = originalTo.X - current.X;
            float origMinusCurrent_y = originalTo.Y - current.Y;
            float origMinusCurrent_z = originalTo.Z - current.Z;
            float outMinusOrig_x = output_x - originalTo.X;
            float outMinusOrig_y = output_y - originalTo.Y;
            float outMinusOrig_z = output_z - originalTo.Z;

            if (origMinusCurrent_x * outMinusOrig_x + origMinusCurrent_y * outMinusOrig_y + origMinusCurrent_z * outMinusOrig_z > 0)
            {
                output_x = originalTo.X;
                output_y = originalTo.Y;
                output_z = originalTo.Z;

                currentVelocity.X = (output_x - originalTo.X) / deltaTime;
                currentVelocity.Y = (output_y - originalTo.Y) / deltaTime;
                currentVelocity.Z = (output_z - originalTo.Z) / deltaTime;
            }

            return new Vector3(output_x, output_y, output_z);
        }

        public static Vector3 Scale(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X * b.X, a.Y * b.Y, a.Z * b.Z);
        }

        public void Scale(Vector3 scale)
        {
            X *= scale.X;
            Y *= scale.Y;
            Z *= scale.Z;
        }

        public static Vector3 Cross(Vector3 lhs, Vector3 rhs)
        {
            return new Vector3(
                lhs.Y * rhs.Z - lhs.Z * rhs.Y,
                lhs.Z * rhs.X - lhs.X * rhs.Z,
                lhs.X * rhs.Y - lhs.Y * rhs.X);
        }

        public override int GetHashCode()
        {
            return X.GetHashCode() ^ (Y.GetHashCode() << 2) ^ (Z.GetHashCode() >> 2);
        }

        public override bool Equals(object other)
        {
            if (!(other is Vector3))
            {
                return false;
            }

            return Equals((Vector3)other);
        }

        public bool Equals(Vector3 other)
        {
            return X == other.X && Y == other.Y && Z == other.Z;
        }

        public static Vector3 Reflect(Vector3 inDirection, Vector3 inNormal)
        {
            float factor = -2.0f * Dot(inNormal, inDirection);
            return new Vector3(factor * inNormal.X + inDirection.X,
                factor * inNormal.Y + inDirection.Y,
                factor * inNormal.Z + inDirection.Z);
        }

        public static Vector3 Normalize(Vector3 value)
        {
            float mag = value.Magnitude;

            if (mag > kEpsilon)
            {
                return value / mag;
            }
            else
            {
                return Zero;
            }
        }

        public void Normalize()
        {
            float mag = Magnitude;

            if (mag > kEpsilon)
            {
                this = this / mag;
            }
            else
            {
                this = Zero;
            }
        }

        public static float Dot(Vector3 lhs, Vector3 rhs)
        {
            return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;
        }

        public static Vector3 Project(Vector3 vector, Vector3 onNormal)
        {
            float sqrMag = Dot(onNormal, onNormal);

            if (sqrMag < MathF.Epsilon)
            {
                return Zero;
            }
            else
            {
                var dot = Dot(vector, onNormal);
                return new Vector3(
                    onNormal.X * dot / sqrMag,
                    onNormal.Y * dot / sqrMag,
                    onNormal.Z * dot / sqrMag);
            }
        }

        public static Vector3 ProjectOnPlane(Vector3 vector, Vector3 planeNormal)
        {
            float sqrMag = Dot(planeNormal, planeNormal);

            if (sqrMag < MathF.Epsilon)
            {
                return vector;
            }
            else
            {
                var dot = Dot(vector, planeNormal);
                return new Vector3(
                    vector.X - planeNormal.X * dot / sqrMag,
                    vector.Y - planeNormal.Y * dot / sqrMag,
                    vector.Z - planeNormal.Z * dot / sqrMag);
            }
        }

        public static float Angle(Vector3 from, Vector3 to)
        {
            // sqrt(a) * sqrt(b) = sqrt(a * b) -- valid for real numbers
            float denominator = (float)Math.Sqrt(from.MagnitudeSquared * to.MagnitudeSquared);

            if (denominator < kEpsilonNormalSqrt)
            {
                return 0.0f;
            }

            float dot = MathF.Clamp(Dot(from, to) / denominator, -1.0f, 1.0f);
            return ((float)Math.Acos(dot)) * MathF.Rad2Deg;
        }

        // The smaller of the two possible angles between the two vectors is returned, therefore the result will never be greater than 180 degrees or smaller than -180 degrees.
        // If you imagine the from and to vectors as lines on a piece of paper, both originating from the same point, then the /axis/ vector would point up out of the paper.
        // The measured angle between the two vectors would be positive in a clockwise direction and negative in an anti-clockwise direction.
        public static float SignedAngle(Vector3 from, Vector3 to, Vector3 axis)
        {
            float unsignedAngle = Angle(from, to);

            float cross_x = from.Y * to.Z - from.Z * to.Y;
            float cross_y = from.Z * to.X - from.X * to.Z;
            float cross_z = from.X * to.Y - from.Y * to.X;
            float sign = MathF.Sign(axis.X * cross_x + axis.Y * cross_y + axis.Z * cross_z);
            return unsignedAngle * sign;
        }

        public static float Distance(Vector3 a, Vector3 b)
        {
            float diff_x = a.X - b.X;
            float diff_y = a.Y - b.Y;
            float diff_z = a.Z - b.Z;
            return (float)Math.Sqrt(diff_x * diff_x + diff_y * diff_y + diff_z * diff_z);
        }

        public static Vector3 ClampMagnitude(Vector3 vector, float maxLength)
        {
            float sqrmag = vector.MagnitudeSquared;

            if (sqrmag > maxLength * maxLength)
            {
                float mag = (float)Math.Sqrt(sqrmag);
                //these intermediate variables force the intermediate result to be
                //of float precision. without this, the intermediate result can be of higher
                //precision, which changes behavior.
                float normalized_x = vector.X / mag;
                float normalized_y = vector.Y / mag;
                float normalized_z = vector.Z / mag;
                return new Vector3(
                    normalized_x * maxLength,
                    normalized_y * maxLength,
                    normalized_z * maxLength);
            }

            return vector;
        }

        public static Vector3 Min(Vector3 lhs, Vector3 rhs)
        {
            return new Vector3(MathF.Min(lhs.X, rhs.X), MathF.Min(lhs.Y, rhs.Y), MathF.Min(lhs.Z, rhs.Z));
        }

        public static Vector3 Max(Vector3 lhs, Vector3 rhs)
        {
            return new Vector3(MathF.Max(lhs.X, rhs.X), MathF.Max(lhs.Y, rhs.Y), MathF.Max(lhs.Z, rhs.Z));
        }

        public float this[int index]
        {
            get
            {
                switch (index)
                {
                    case 0: return X;
                    case 1: return Y;
                    case 2: return Z;
                    default:
                        throw new IndexOutOfRangeException("Invalid Vector3 index!");
                }
            }

            set
            {
                switch (index)
                {
                    case 0: X = value; break;
                    case 1: Y = value; break;
                    case 2: Z = value; break;
                    default:
                        throw new IndexOutOfRangeException("Invalid Vector3 index!");
                }
            }
        }

        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        }

        public static Vector3 operator -(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
        }

        public static Vector3 operator -(Vector3 a)
        {
            return new Vector3(-a.X, -a.Y, -a.Z);
        }

        public static Vector3 operator *(Vector3 a, float d)
        {
            return new Vector3(a.X * d, a.Y * d, a.Z * d);
        }

        public static Vector3 operator *(float d, Vector3 a)
        {
            return new Vector3(a.X * d, a.Y * d, a.Z * d);
        }

        public static Vector3 operator *(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X * b.X, a.Y * b.Y, a.Z * b.Z);
        }

        public static Vector3 operator *(Vector3 lhs, Matrix rhs)
        {
            return Internal_TransformVector3(ref lhs, ref rhs);
        }

        public static Vector3 operator /(Vector3 a, float d)
        {
            return new Vector3(a.X / d, a.Y / d, a.Z / d);
        }

        public static Vector3 operator /(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X / b.X, a.Y / b.Y, a.Z / b.Z);
        }

        public static bool operator ==(Vector3 lhs, Vector3 rhs)
        {
            // Returns false in the presence of NaN values.
            float diff_x = lhs.X - rhs.X;
            float diff_y = lhs.Y - rhs.Y;
            float diff_z = lhs.Z - rhs.Z;
            float sqrmag = diff_x * diff_x + diff_y * diff_y + diff_z * diff_z;
            return sqrmag < kEpsilon * kEpsilon;
        }

        public static bool operator !=(Vector3 lhs, Vector3 rhs)
        {
            // Returns true in the presence of NaN values.
            return !(lhs == rhs);
        }

        public override string ToString()
        {
            return ToString(null, null);
        }

        public string ToString(string format)
        {
            return ToString(format, null);
        }

        public string ToString(string format, IFormatProvider formatProvider)
        {
            if (string.IsNullOrEmpty(format))
            {
                format = "F2";
            }

            if (formatProvider == null)
            {
                formatProvider = CultureInfo.InvariantCulture.NumberFormat;
            }

            return string.Format("({0}, {1}, {2})", X.ToString(format, formatProvider), Y.ToString(format, formatProvider), Z.ToString(format, formatProvider));
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static Vector3 Internal_TransformVector3(ref Vector3 vector, ref Matrix matrix);
    }
}


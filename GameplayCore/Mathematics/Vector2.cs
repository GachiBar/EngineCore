using System;
using System.Globalization;

namespace GameplayCore.Mathematics
{
    public struct Vector2 : IEquatable<Vector2>, IFormattable
    {
        public const float kEpsilon = 0.00001F;
        public const float kEpsilonNormalSqrt = 1e-15f;

        public float X;
        public float Y;

        public static Vector2 Zero => new Vector2(0.0f, 0.0f);
        public static Vector2 One => new Vector2(1.0f, 1.0f);
        public static Vector2 Up => new Vector2(0.0f, 1.0f);
        public static Vector2 Down => new Vector2(0.0f, -1.0f);
        public static Vector2 Left => new Vector2(-1.0f, 0.0f);
        public static Vector2 Right => new Vector2(1.0f, 0.0f);
        public static Vector2 PositiveInfinity => new Vector2(float.PositiveInfinity, float.PositiveInfinity);
        public static Vector2 NegativeInfinity => new Vector2(float.NegativeInfinity, float.NegativeInfinity);

        public Vector2 Normalized
        {
            get
            {
                Vector2 v = new Vector2(X, Y);
                v.Normalize();
                return v;
            }
        }

        public float Magnitude => (float)Math.Sqrt(X * X + Y * Y);
        public float MagnitudeSquared => X * X + Y * Y;

        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }

        public void Set(float newX, float newY)
        {
            X = newX;
            Y = newY;
        }

        public static Vector2 Lerp(Vector2 a, Vector2 b, float t)
        {
            t = MathF.Clamp01(t);
            return new Vector2(
                a.X + (b.X - a.X) * t,
                a.Y + (b.Y - a.Y) * t);
        }

        public static Vector2 LerpUnclamped(Vector2 a, Vector2 b, float t)
        {
            return new Vector2(
                a.X + (b.X - a.X) * t,
                a.Y + (b.Y - a.Y) * t);
        }

        public static Vector2 MoveTowards(Vector2 current, Vector2 target, float maxDistanceDelta)
        {
            // avoid vector ops because current scripting backends are terrible at inlining
            float toVector_x = target.X - current.X;
            float toVector_y = target.Y - current.Y;

            float sqDist = toVector_x * toVector_x + toVector_y * toVector_y;

            if (sqDist == 0 || (maxDistanceDelta >= 0 && sqDist <= maxDistanceDelta * maxDistanceDelta))
            {
                return target;
            }

            float dist = (float)Math.Sqrt(sqDist);

            return new Vector2(
                current.X + toVector_x / dist * maxDistanceDelta,
                current.Y + toVector_y / dist * maxDistanceDelta);
        }

        public static Vector2 Scale(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X * b.X, a.Y * b.Y);
        }

        public void Scale(Vector2 scale)
        {
            X *= scale.X;
            Y *= scale.Y;
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

            return string.Format("({0}, {1})", X.ToString(format, formatProvider), Y.ToString(format, formatProvider));
        }

        public override int GetHashCode()
        {
            return X.GetHashCode() ^ (Y.GetHashCode() << 2);
        }

        public override bool Equals(object other)
        {
            if (!(other is Vector2))
            {
                return false;
            }

            return Equals((Vector2)other);
        }

        public bool Equals(Vector2 other)
        {
            return X == other.X && Y == other.Y;
        }

        public static Vector2 Reflect(Vector2 inDirection, Vector2 inNormal)
        {
            float factor = -2F * Dot(inNormal, inDirection);
            return new Vector2(factor * inNormal.X + inDirection.X, factor * inNormal.Y + inDirection.Y);
        }

        public static Vector2 Perpendicular(Vector2 inDirection)
        {
            return new Vector2(-inDirection.Y, inDirection.X);
        }

        public static float Dot(Vector2 lhs, Vector2 rhs)
        {
            return lhs.X * rhs.X + lhs.Y * rhs.Y;
        }

        public static float Angle(Vector2 from, Vector2 to)
        {
            // sqrt(a) * sqrt(b) = sqrt(a * b) -- valid for real numbers
            float denominator = (float)Math.Sqrt(from.MagnitudeSquared * to.MagnitudeSquared);

            if (denominator < kEpsilonNormalSqrt)
            {
                return 0F;
            }

            float dot = MathF.Clamp(Dot(from, to) / denominator, -1F, 1F);
            return (float)Math.Acos(dot) * MathF.Rad2Deg;
        }

        public static float SignedAngle(Vector2 from, Vector2 to)
        {
            float unsigned_angle = Angle(from, to);
            float sign = MathF.Sign(from.X * to.Y - from.Y * to.X);
            return unsigned_angle * sign;
        }

        public static float Distance(Vector2 a, Vector2 b)
        {
            float diff_x = a.X - b.X;
            float diff_y = a.Y - b.Y;
            return (float)Math.Sqrt(diff_x * diff_x + diff_y * diff_y);
        }

        public static Vector2 ClampMagnitude(Vector2 vector, float maxLength)
        {
            float sqrMagnitude = vector.MagnitudeSquared;

            if (sqrMagnitude > maxLength * maxLength)
            {
                float mag = (float)Math.Sqrt(sqrMagnitude);

                //these intermediate variables force the intermediate result to be
                //of float precision. without this, the intermediate result can be of higher
                //precision, which changes behavior.
                float normalized_x = vector.X / mag;
                float normalized_y = vector.Y / mag;
                return new Vector2(normalized_x * maxLength, normalized_y * maxLength);
            }

            return vector;
        }

        public static float SqrMagnitude(Vector2 a)
        {
            return a.X * a.X + a.Y * a.Y;
        }

        public float SqrMagnitude()
        {
            return X * X + Y * Y;
        }

        public static Vector2 Min(Vector2 lhs, Vector2 rhs)
        {
            return new Vector2(MathF.Min(lhs.X, rhs.X), MathF.Min(lhs.Y, rhs.Y));
        }

        public static Vector2 Max(Vector2 lhs, Vector2 rhs)
        {
            return new Vector2(MathF.Max(lhs.X, rhs.X), MathF.Max(lhs.Y, rhs.Y));
        }

        //public static Vector2 SmoothDamp(Vector2 current, Vector2 target, ref Vector2 currentVelocity, float smoothTime, float maxSpeed)
        //{
        //    float deltaTime = Time.deltaTime;
        //    return SmoothDamp(current, target, ref currentVelocity, smoothTime, maxSpeed, deltaTime);
        //}

        //public static Vector2 SmoothDamp(Vector2 current, Vector2 target, ref Vector2 currentVelocity, float smoothTime)
        //{
        //    float deltaTime = Time.deltaTime;
        //    float maxSpeed = Mathf.Infinity;
        //    return SmoothDamp(current, target, ref currentVelocity, smoothTime, maxSpeed, deltaTime);
        //}

        public static Vector2 SmoothDamp(Vector2 current, Vector2 target, ref Vector2 currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
        {
            // Based on Game Programming Gems 4 Chapter 1.10
            smoothTime = MathF.Max(0.0001F, smoothTime);
            float omega = 2F / smoothTime;

            float x = omega * deltaTime;
            float exp = 1F / (1F + x + 0.48F * x * x + 0.235F * x * x * x);

            float change_x = current.X - target.X;
            float change_y = current.Y - target.Y;
            Vector2 originalTo = target;

            // Clamp maximum speed
            float maxChange = maxSpeed * smoothTime;

            float maxChangeSq = maxChange * maxChange;
            float sqDist = change_x * change_x + change_y * change_y;

            if (sqDist > maxChangeSq)
            {
                var mag = (float)Math.Sqrt(sqDist);
                change_x = change_x / mag * maxChange;
                change_y = change_y / mag * maxChange;
            }

            target.X = current.X - change_x;
            target.Y = current.Y - change_y;

            float temp_x = (currentVelocity.X + omega * change_x) * deltaTime;
            float temp_y = (currentVelocity.Y + omega * change_y) * deltaTime;

            currentVelocity.X = (currentVelocity.X - omega * temp_x) * exp;
            currentVelocity.Y = (currentVelocity.Y - omega * temp_y) * exp;

            float output_x = target.X + (change_x + temp_x) * exp;
            float output_y = target.Y + (change_y + temp_y) * exp;

            // Prevent overshooting
            float origMinusCurrent_x = originalTo.X - current.X;
            float origMinusCurrent_y = originalTo.Y - current.Y;
            float outMinusOrig_x = output_x - originalTo.X;
            float outMinusOrig_y = output_y - originalTo.Y;

            if (origMinusCurrent_x * outMinusOrig_x + origMinusCurrent_y * outMinusOrig_y > 0)
            {
                output_x = originalTo.X;
                output_y = originalTo.Y;

                currentVelocity.X = (output_x - originalTo.X) / deltaTime;
                currentVelocity.Y = (output_y - originalTo.Y) / deltaTime;
            }

            return new Vector2(output_x, output_y);
        }

        public float this[int index]
        {
            get
            {
                switch (index)
                {
                    case 0: return X;
                    case 1: return Y;
                    default:
                        throw new IndexOutOfRangeException("Invalid Vector2 index!");
                }
            }

            set
            {
                switch (index)
                {
                    case 0: X = value; break;
                    case 1: Y = value; break;
                    default:
                        throw new IndexOutOfRangeException("Invalid Vector2 index!");
                }
            }
        }

        public static Vector2 operator +(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X + b.X, a.Y + b.Y);
        }


        public static Vector2 operator -(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X - b.X, a.Y - b.Y);
        }


        public static Vector2 operator *(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X * b.X, a.Y * b.Y);
        }


        public static Vector2 operator /(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X / b.X, a.Y / b.Y);
        }

        public static Vector2 operator -(Vector2 a)
        {
            return new Vector2(-a.X, -a.Y);
        }

        public static Vector2 operator *(Vector2 a, float d)
        {
            return new Vector2(a.X * d, a.Y * d);
        }

        public static Vector2 operator *(float d, Vector2 a)
        {
            return new Vector2(a.X * d, a.Y * d);
        }

        public static Vector2 operator /(Vector2 a, float d)
        {
            return new Vector2(a.X / d, a.Y / d);
        }

        public static bool operator ==(Vector2 lhs, Vector2 rhs)
        {
            // Returns false in the presence of NaN values.
            float diff_x = lhs.X - rhs.X;
            float diff_y = lhs.Y - rhs.Y;
            return (diff_x * diff_x + diff_y * diff_y) < kEpsilon * kEpsilon;
        }

        public static bool operator !=(Vector2 lhs, Vector2 rhs)
        {
            // Returns true in the presence of NaN values.
            return !(lhs == rhs);
        }

        public static implicit operator Vector2(Vector3 v)
        {
            return new Vector2(v.X, v.Y);
        }

        public static implicit operator Vector3(Vector2 v)
        {
            return new Vector3(v.X, v.Y, 0);
        }
    }
}

using System;
using System.Globalization;

namespace GameplayCore.Mathematics
{
    public partial struct Vector4 : IEquatable<Vector4>, IFormattable
    {
        public const float kEpsilon = 0.00001f;

        public float X;
        public float Y;
        public float Z;
        public float W;

        public static Vector4 Zero => new Vector4(0.0f, 0.0f, 0.0f, 0.0f);
        public static Vector4 One => new Vector4(0.0f, 0.0f, 0.0f, 0.0f);
        public static Vector4 PositiveInfinity => new Vector4(float.PositiveInfinity, float.PositiveInfinity, float.PositiveInfinity, float.PositiveInfinity);
        public static Vector4 NegativeInfinity => new Vector4(float.NegativeInfinity, float.NegativeInfinity, float.NegativeInfinity, float.NegativeInfinity);

        public Vector4 Normalized => Vector4.Normalize(this);
        public float Magnitude => (float)Math.Sqrt(Dot(this, this));
        public float MagnitudeSquared => Dot(this, this);

        public Vector4(float x, float y, float z, float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        public Vector4(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
            W = 0.0f;
        }

        public Vector4(float x, float y)
        {
            this.X = x;
            this.Y = y;
            this.Z = 0.0f;
            this.W = 0.0f;
        }

        public void Set(float newX, float newY, float newZ, float newW)
        {
            X = newX;
            Y = newY;
            Z = newZ;
            W = newW;
        }

        public static Vector4 Lerp(Vector4 a, Vector4 b, float t)
        {
            t = MathF.Clamp01(t);
            return new Vector4(
                a.X + (b.X - a.X) * t,
                a.Y + (b.Y - a.Y) * t,
                a.Z + (b.Z - a.Z) * t,
                a.W + (b.W - a.W) * t);
        }

        public static Vector4 LerpUnclamped(Vector4 a, Vector4 b, float t)
        {
            return new Vector4(
                a.X + (b.X - a.X) * t,
                a.Y + (b.Y - a.Y) * t,
                a.Z + (b.Z - a.Z) * t,
                a.W + (b.W - a.W) * t);
        }

        public static Vector4 MoveTowards(Vector4 current, Vector4 target, float maxDistanceDelta)
        {
            float toVector_x = target.X - current.X;
            float toVector_y = target.Y - current.Y;
            float toVector_z = target.Z - current.Z;
            float toVector_w = target.W - current.W;

            float sqdist = (toVector_x * toVector_x +
                toVector_y * toVector_y +
                toVector_z * toVector_z +
                toVector_w * toVector_w);

            if (sqdist == 0 || (maxDistanceDelta >= 0 && sqdist <= maxDistanceDelta * maxDistanceDelta))
            {
                return target;
            }

            var dist = (float)Math.Sqrt(sqdist);

            return new Vector4(current.X + toVector_x / dist * maxDistanceDelta,
                current.Y + toVector_y / dist * maxDistanceDelta,
                current.Z + toVector_z / dist * maxDistanceDelta,
                current.W + toVector_w / dist * maxDistanceDelta);
        }

        public static Vector4 Scale(Vector4 a, Vector4 b)
        {
            return new Vector4(a.X * b.X, a.Y * b.Y, a.Z * b.Z, a.W * b.W);
        }

        public void Scale(Vector4 scale)
        {
            X *= scale.X;
            Y *= scale.Y;
            Z *= scale.Z;
            W *= scale.W;
        }

        public override int GetHashCode()
        {
            return X.GetHashCode() ^ (Y.GetHashCode() << 2) ^ (Z.GetHashCode() >> 2) ^ (W.GetHashCode() >> 1);
        }

        public override bool Equals(object other)
        {
            if (!(other is Vector4))
            {
                return false;
            }

            return Equals((Vector4)other);
        }

        public bool Equals(Vector4 other)
        {
            return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
        }

        public static Vector4 Normalize(Vector4 a)
        {
            float mag = a.Magnitude;

            if (mag > kEpsilon)
            {
                return a / mag;
            }

            return Zero;
        }

        public void Normalize()
        {
            float mag = this.Magnitude;

            if (mag > kEpsilon)
            {
                this = this / mag;
            }

            this = Zero;
        }

        public static float Dot(Vector4 a, Vector4 b)
        {
            return a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W;
        }

        public static Vector4 Project(Vector4 a, Vector4 b)
        {
            return b * (Dot(a, b) / Dot(b, b));
        }

        public static float Distance(Vector4 a, Vector4 b)
        {
            return (a - b).Magnitude;
        }

        public static Vector4 Min(Vector4 lhs, Vector4 rhs)
        {
            return new Vector4(MathF.Min(lhs.X, rhs.X), MathF.Min(lhs.Y, rhs.Y), MathF.Min(lhs.Z, rhs.Z), MathF.Min(lhs.W, rhs.W));
        }

        public static Vector4 Max(Vector4 lhs, Vector4 rhs)
        {
            return new Vector4(MathF.Max(lhs.X, rhs.X), MathF.Max(lhs.Y, rhs.Y), MathF.Max(lhs.Z, rhs.Z), MathF.Max(lhs.W, rhs.W));
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
                    case 3: return W;
                    default:
                        throw new IndexOutOfRangeException("Invalid Vector4 index!");
                }
            }

            set
            {
                switch (index)
                {
                    case 0: X = value; break;
                    case 1: Y = value; break;
                    case 2: Z = value; break;
                    case 3: W = value; break;
                    default:
                        throw new IndexOutOfRangeException("Invalid Vector4 index!");
                }
            }
        }

        public static Vector4 operator +(Vector4 a, Vector4 b)
        {
            return new Vector4(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W + b.W);
        }

        public static Vector4 operator -(Vector4 a, Vector4 b)
        {
            return new Vector4(a.X - b.X, a.Y - b.Y, a.Z - b.Z, a.W - b.W);
        }

        public static Vector4 operator -(Vector4 a)
        {
            return new Vector4(-a.X, -a.Y, -a.Z, -a.W);
        }

        public static Vector4 operator *(Vector4 a, float d)
        {
            return new Vector4(a.X * d, a.Y * d, a.Z * d, a.W * d);
        }

        public static Vector4 operator *(float d, Vector4 a)
        {
            return new Vector4(a.X * d, a.Y * d, a.Z * d, a.W * d);
        }

        public static Vector4 operator /(Vector4 a, float d)
        {
            return new Vector4(a.X / d, a.Y / d, a.Z / d, a.W / d);
        }

        public static bool operator ==(Vector4 lhs, Vector4 rhs)
        {
            // Returns false in the presence of NaN values.
            float diffx = lhs.X - rhs.X;
            float diffy = lhs.Y - rhs.Y;
            float diffz = lhs.Z - rhs.Z;
            float diffw = lhs.W - rhs.W;
            float sqrmag = diffx * diffx + diffy * diffy + diffz * diffz + diffw * diffw;
            return sqrmag < kEpsilon * kEpsilon;
        }

        public static bool operator !=(Vector4 lhs, Vector4 rhs)
        {
            // Returns true in the presence of NaN values.
            return !(lhs == rhs);
        }

        public static implicit operator Vector4(Vector3 v)
        {
            return new Vector4(v.X, v.Y, v.Z, 0.0f);
        }

        public static implicit operator Vector3(Vector4 v)
        {
            return new Vector3(v.X, v.Y, v.Z);
        }

        public static implicit operator Vector4(Vector2 v)
        {
            return new Vector4(v.X, v.Y, 0.0f, 0.0f);
        }

        public static implicit operator Vector2(Vector4 v)
        {
            return new Vector2(v.X, v.Y);
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

            return string.Format("({0}, {1}, {2}, {3})", X.ToString(format, formatProvider), Y.ToString(format, formatProvider), Z.ToString(format, formatProvider), W.ToString(format, formatProvider));
        }
    }
}

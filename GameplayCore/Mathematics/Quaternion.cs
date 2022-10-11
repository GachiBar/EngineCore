using System;
using System.Globalization;
using System.Runtime.CompilerServices;

namespace GameplayCore.Mathematics
{
    public partial struct Quaternion : IEquatable<Quaternion>, IFormattable
    {
        public const float kEpsilon = 0.000001f;

        public float X;
        public float Y;
        public float Z;
        public float W;

        public static Quaternion Identity => new Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

        public Quaternion Normalized => Normalize(this);

        public Vector3 EulerAngles
        {
            get => Internal_MakePositive(Internal_ToEulerRad(this) * MathF.Rad2Deg);
            set => this = Internal_FromEulerRad(value * MathF.Deg2Rad);
        }

        public Quaternion(float x, float y, float z, float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        public void Set(float newX, float newY, float newZ, float newW)
        {
            X = newX;
            Y = newY;
            Z = newZ;
            W = newW;
        }

        public static Quaternion operator *(Quaternion lhs, Quaternion rhs)
        {
            return new Quaternion(
                lhs.W * rhs.X + lhs.X * rhs.W + lhs.Y * rhs.Z - lhs.Z * rhs.Y,
                lhs.W * rhs.Y + lhs.Y * rhs.W + lhs.Z * rhs.X - lhs.X * rhs.Z,
                lhs.W * rhs.Z + lhs.Z * rhs.W + lhs.X * rhs.Y - lhs.Y * rhs.X,
                lhs.W * rhs.W - lhs.X * rhs.X - lhs.Y * rhs.Y - lhs.Z * rhs.Z);
        }

        public static Vector3 operator *(Quaternion rotation, Vector3 point)
        {
            float x = rotation.X * 2F;
            float y = rotation.Y * 2F;
            float z = rotation.Z * 2F;
            float xx = rotation.X * x;
            float yy = rotation.Y * y;
            float zz = rotation.Z * z;
            float xy = rotation.X * y;
            float xz = rotation.X * z;
            float yz = rotation.Y * z;
            float wx = rotation.W * x;
            float wy = rotation.W * y;
            float wz = rotation.W * z;

            Vector3 res;
            res.X = (1F - (yy + zz)) * point.X + (xy - wz) * point.Y + (xz + wy) * point.Z;
            res.Y = (xy + wz) * point.X + (1F - (xx + zz)) * point.Y + (yz - wx) * point.Z;
            res.Z = (xz - wy) * point.X + (yz + wx) * point.Y + (1F - (xx + yy)) * point.Z;
            return res;
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
                        throw new IndexOutOfRangeException("Invalid Quaternion index!");
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
                        throw new IndexOutOfRangeException("Invalid Quaternion index!");
                }
            }
        }

        public static bool operator ==(Quaternion lhs, Quaternion rhs)
        {
            return IsEqualUsingDot(Dot(lhs, rhs));
        }

        public static bool operator !=(Quaternion lhs, Quaternion rhs)
        {
            // Returns true in the presence of NaN values.
            return !(lhs == rhs);
        }

        public static float Dot(Quaternion a, Quaternion b)
        {
            return a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W;
        }

        public void SetLookRotation(Vector3 view)
        {
            Vector3 up = Vector3.Up;
            SetLookRotation(view, up);
        }

        public void SetLookRotation(Vector3 view, Vector3 up)
        {
            this = LookRotation(view, up);
        }

        public static float Angle(Quaternion a, Quaternion b)
        {
            float dot = MathF.Min(MathF.Abs(Dot(a, b)), 1.0f);
            return IsEqualUsingDot(dot) ? 0.0f : MathF.Acos(dot) * 2.0f * MathF.Rad2Deg;
        }

        public static Quaternion Euler(float x, float y, float z)
        {
            return Internal_FromEulerRad(new Vector3(x, y, z) * MathF.Deg2Rad);
        }

        public static Quaternion Euler(Vector3 euler)
        {
            return Internal_FromEulerRad(euler * MathF.Deg2Rad);
        }

        public void ToAngleAxis(out float angle, out Vector3 axis)
        {
            Internal_ToAxisAngleRad(this, out axis, out angle);
            angle *= MathF.Rad2Deg;
        }

        public void SetFromToRotation(Vector3 fromDirection, Vector3 toDirection)
        {
            this = FromToRotation(fromDirection, toDirection);
        }

        public static Quaternion RotateTowards(Quaternion from, Quaternion to, float maxDegreesDelta)
        {
            float angle = Quaternion.Angle(from, to);

            if (angle == 0.0f)
            {
                return to;
            }

            return Internal_Slerp(from, to, MathF.Min(1.0f, maxDegreesDelta / angle));
        }

        public static Quaternion Normalize(Quaternion q)
        {
            float mag = MathF.Sqrt(Dot(q, q));

            if (mag < MathF.Epsilon)
            {
                return Quaternion.Identity;
            }

            return new Quaternion(q.X / mag, q.Y / mag, q.Z / mag, q.W / mag);
        }

        public void Normalize()
        {
            this = Normalize(this);
        }

        public override int GetHashCode()
        {
            return X.GetHashCode() ^ (Y.GetHashCode() << 2) ^ (Z.GetHashCode() >> 2) ^ (W.GetHashCode() >> 1);
        }

        // also required for being able to use Quaternions as keys in hash tables
        public override bool Equals(object other)
        {
            if (!(other is Quaternion))
            {
                return false;
            }

            return Equals((Quaternion)other);
        }

        public bool Equals(Quaternion other)
        {
            return X.Equals(other.X) && Y.Equals(other.Y) && Z.Equals(other.Z) && W.Equals(other.W);
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
                format = "F5";
            }

            if (formatProvider == null)
            {
                formatProvider = CultureInfo.InvariantCulture.NumberFormat;
            }

            return string.Format("({0}, {1}, {2}, {3})", X.ToString(format, formatProvider), Y.ToString(format, formatProvider), Z.ToString(format, formatProvider), W.ToString(format, formatProvider));
        }

        private static bool IsEqualUsingDot(float dot)
        {
            // Returns false in the presence of NaN values.
            return dot > 1.0f - kEpsilon;
        }

        // Makes euler angles positive 0/360 with 0.0001 hacked to support old behaviour of QuaternionToEuler
        private static Vector3 Internal_MakePositive(Vector3 euler)
        {
            float negativeFlip = -0.0001f * MathF.Rad2Deg;
            float positiveFlip = 360.0f + negativeFlip;

            if (euler.X < negativeFlip)
            {
                euler.X += 360.0f;
            }
            else if (euler.X > positiveFlip)
            {
                euler.X -= 360.0f;
            }

            if (euler.Y < negativeFlip)
            {
                euler.Y += 360.0f;
            }
            else if (euler.Y > positiveFlip)
            {
                euler.Y -= 360.0f;
            }

            if (euler.Z < negativeFlip)
            {
                euler.Z += 360.0f;
            }
            else if (euler.Z > positiveFlip)
            {
                euler.Z -= 360.0f;
            }

            return euler;
        }

        public static Quaternion LookRotation(Vector3 forward)
        {
            return Internal_LookRotation(forward, Vector3.Up);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static Quaternion Internal_Slerp(Quaternion q1, Quaternion q2, float t);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static Quaternion Internal_LookRotation(Vector3 forward, Vector3 upwards);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static Quaternion Internal_FromEulerRad(Vector3 euler);
    }
}
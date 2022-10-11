using System;
using System.Globalization;

namespace GameplayCore.Mathematics
{
    public partial struct Matrix : IEquatable<Matrix>, IFormattable
    {
        // memory layout:
        //
        //                row no (=vertical)
        //               |  0   1   2   3
        //            ---+----------------
        //            0  | m00 m10 m20 m30
        // column no  1  | m01 m11 m21 m31
        // (=horiz)   2  | m02 m12 m22 m32
        //            3  | m03 m13 m23 m33

        public float m00;
        public float m10;
        public float m20;
        public float m30;
        public float m01;
        public float m11;
        public float m21;
        public float m31;
        public float m02;
        public float m12;
        public float m22;
        public float m32;
        public float m03;
        public float m13;
        public float m23;
        public float m33;

        public Matrix(Vector4 column0, Vector4 column1, Vector4 column2, Vector4 column3)
        {
            m00 = column0.X; m01 = column1.X; m02 = column2.X; m03 = column3.X;
            m10 = column0.Y; m11 = column1.Y; m12 = column2.Y; m13 = column3.Y;
            m20 = column0.Z; m21 = column1.Z; m22 = column2.Z; m23 = column3.Z;
            m30 = column0.W; m31 = column1.W; m32 = column2.W; m33 = column3.W;
        }

        public float this[int row, int column]
        {
            get => this[row + column * 4];
            set => this[row + column * 4] = value;
        }

        public float this[int index]
        {
            get
            {
                switch (index)
                {
                    case 0: return m00;
                    case 1: return m10;
                    case 2: return m20;
                    case 3: return m30;
                    case 4: return m01;
                    case 5: return m11;
                    case 6: return m21;
                    case 7: return m31;
                    case 8: return m02;
                    case 9: return m12;
                    case 10: return m22;
                    case 11: return m32;
                    case 12: return m03;
                    case 13: return m13;
                    case 14: return m23;
                    case 15: return m33;
                    default:
                        throw new IndexOutOfRangeException("Invalid matrix index!");
                }
            }

            set
            {
                switch (index)
                {
                    case 0: m00 = value; break;
                    case 1: m10 = value; break;
                    case 2: m20 = value; break;
                    case 3: m30 = value; break;
                    case 4: m01 = value; break;
                    case 5: m11 = value; break;
                    case 6: m21 = value; break;
                    case 7: m31 = value; break;
                    case 8: m02 = value; break;
                    case 9: m12 = value; break;
                    case 10: m22 = value; break;
                    case 11: m32 = value; break;
                    case 12: m03 = value; break;
                    case 13: m13 = value; break;
                    case 14: m23 = value; break;
                    case 15: m33 = value; break;

                    default:
                        throw new IndexOutOfRangeException("Invalid matrix index!");
                }
            }
        }

        public override int GetHashCode()
        {
            return GetColumn(0).GetHashCode() ^ (GetColumn(1).GetHashCode() << 2) ^ (GetColumn(2).GetHashCode() >> 2) ^ (GetColumn(3).GetHashCode() >> 1);
        }

        public override bool Equals(object other)
        {
            if (!(other is Matrix))
            {
                return false;
            }

            return Equals((Matrix)other);
        }

        public bool Equals(Matrix other)
        {
            return GetColumn(0).Equals(other.GetColumn(0))
                && GetColumn(1).Equals(other.GetColumn(1))
                && GetColumn(2).Equals(other.GetColumn(2))
                && GetColumn(3).Equals(other.GetColumn(3));
        }

        public static Matrix operator *(Matrix lhs, Matrix rhs)
        {
            Matrix res;
            res.m00 = lhs.m00 * rhs.m00 + lhs.m01 * rhs.m10 + lhs.m02 * rhs.m20 + lhs.m03 * rhs.m30;
            res.m01 = lhs.m00 * rhs.m01 + lhs.m01 * rhs.m11 + lhs.m02 * rhs.m21 + lhs.m03 * rhs.m31;
            res.m02 = lhs.m00 * rhs.m02 + lhs.m01 * rhs.m12 + lhs.m02 * rhs.m22 + lhs.m03 * rhs.m32;
            res.m03 = lhs.m00 * rhs.m03 + lhs.m01 * rhs.m13 + lhs.m02 * rhs.m23 + lhs.m03 * rhs.m33;

            res.m10 = lhs.m10 * rhs.m00 + lhs.m11 * rhs.m10 + lhs.m12 * rhs.m20 + lhs.m13 * rhs.m30;
            res.m11 = lhs.m10 * rhs.m01 + lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21 + lhs.m13 * rhs.m31;
            res.m12 = lhs.m10 * rhs.m02 + lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22 + lhs.m13 * rhs.m32;
            res.m13 = lhs.m10 * rhs.m03 + lhs.m11 * rhs.m13 + lhs.m12 * rhs.m23 + lhs.m13 * rhs.m33;

            res.m20 = lhs.m20 * rhs.m00 + lhs.m21 * rhs.m10 + lhs.m22 * rhs.m20 + lhs.m23 * rhs.m30;
            res.m21 = lhs.m20 * rhs.m01 + lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21 + lhs.m23 * rhs.m31;
            res.m22 = lhs.m20 * rhs.m02 + lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22 + lhs.m23 * rhs.m32;
            res.m23 = lhs.m20 * rhs.m03 + lhs.m21 * rhs.m13 + lhs.m22 * rhs.m23 + lhs.m23 * rhs.m33;

            res.m30 = lhs.m30 * rhs.m00 + lhs.m31 * rhs.m10 + lhs.m32 * rhs.m20 + lhs.m33 * rhs.m30;
            res.m31 = lhs.m30 * rhs.m01 + lhs.m31 * rhs.m11 + lhs.m32 * rhs.m21 + lhs.m33 * rhs.m31;
            res.m32 = lhs.m30 * rhs.m02 + lhs.m31 * rhs.m12 + lhs.m32 * rhs.m22 + lhs.m33 * rhs.m32;
            res.m33 = lhs.m30 * rhs.m03 + lhs.m31 * rhs.m13 + lhs.m32 * rhs.m23 + lhs.m33 * rhs.m33;

            return res;
        }

        public static Vector4 operator *(Matrix lhs, Vector4 vector)
        {
            Vector4 res;
            res.X = lhs.m00 * vector.X + lhs.m01 * vector.Y + lhs.m02 * vector.Z + lhs.m03 * vector.W;
            res.Y = lhs.m10 * vector.X + lhs.m11 * vector.Y + lhs.m12 * vector.Z + lhs.m13 * vector.W;
            res.Z = lhs.m20 * vector.X + lhs.m21 * vector.Y + lhs.m22 * vector.Z + lhs.m23 * vector.W;
            res.W = lhs.m30 * vector.X + lhs.m31 * vector.Y + lhs.m32 * vector.Z + lhs.m33 * vector.W;
            return res;
        }

        public static bool operator ==(Matrix lhs, Matrix rhs)
        {
            // Returns false in the presence of NaN values.
            return lhs.GetColumn(0) == rhs.GetColumn(0)
                && lhs.GetColumn(1) == rhs.GetColumn(1)
                && lhs.GetColumn(2) == rhs.GetColumn(2)
                && lhs.GetColumn(3) == rhs.GetColumn(3);
        }

        public static bool operator !=(Matrix lhs, Matrix rhs)
        {
            // Returns true in the presence of NaN values.
            return !(lhs == rhs);
        }

        public Vector4 GetColumn(int index)
        {
            switch (index)
            {
                case 0: return new Vector4(m00, m10, m20, m30);
                case 1: return new Vector4(m01, m11, m21, m31);
                case 2: return new Vector4(m02, m12, m22, m32);
                case 3: return new Vector4(m03, m13, m23, m33);
                default:
                    throw new IndexOutOfRangeException("Invalid column index!");
            }
        }

        public Vector4 GetRow(int index)
        {
            switch (index)
            {
                case 0: return new Vector4(m00, m01, m02, m03);
                case 1: return new Vector4(m10, m11, m12, m13);
                case 2: return new Vector4(m20, m21, m22, m23);
                case 3: return new Vector4(m30, m31, m32, m33);
                default:
                    throw new IndexOutOfRangeException("Invalid row index!");
            }
        }

        public Vector3 GetPosition()
        {
            return new Vector3(m03, m13, m23);
        }

        public void SetColumn(int index, Vector4 column)
        {
            this[0, index] = column.X;
            this[1, index] = column.Y;
            this[2, index] = column.Z;
            this[3, index] = column.W;
        }

        public void SetRow(int index, Vector4 row)
        {
            this[index, 0] = row.X;
            this[index, 1] = row.Y;
            this[index, 2] = row.Z;
            this[index, 3] = row.W;
        }

        public Vector3 MultiplyPoint(Vector3 point)
        {
            Vector3 res;
            float w;
            res.X = m00 * point.X + m01 * point.Y + m02 * point.Z + m03;
            res.Y = m10 * point.X + m11 * point.Y + m12 * point.Z + m13;
            res.Z = m20 * point.X + m21 * point.Y + m22 * point.Z + m23;
            w = m30 * point.X + m31 * point.Y + m32 * point.Z + m33;

            w = 1.0f / w;
            res.X *= w;
            res.Y *= w;
            res.Z *= w;
            return res;
        }

        // Transforms a position by this matrix, without a perspective divide. (fast)
        public Vector3 MultiplyPoint3x4(Vector3 point)
        {
            Vector3 res;
            res.X = m00 * point.X + m01 * point.Y + m02 * point.Z + m03;
            res.Y = m10 * point.X + m11 * point.Y + m12 * point.Z + m13;
            res.Z = m20 * point.X + m21 * point.Y + m22 * point.Z + m23;
            return res;
        }

        // Transforms a direction by this matrix.
        public Vector3 MultiplyVector(Vector3 vector)
        {
            Vector3 res;
            res.X = m00 * vector.X + m01 * vector.Y + m02 * vector.Z;
            res.Y = m10 * vector.X + m11 * vector.Y + m12 * vector.Z;
            res.Z = m20 * vector.X + m21 * vector.Y + m22 * vector.Z;
            return res;
        }

        //public Plane TransformPlane(Plane plane)
        //{
        //    var ittrans = this.inverse;

        //    float x = plane.normal.x, y = plane.normal.y, z = plane.normal.z, w = plane.distance;
        //    // note: a transpose is part of this transformation
        //    var a = ittrans.m00 * x + ittrans.m10 * y + ittrans.m20 * z + ittrans.m30 * w;
        //    var b = ittrans.m01 * x + ittrans.m11 * y + ittrans.m21 * z + ittrans.m31 * w;
        //    var c = ittrans.m02 * x + ittrans.m12 * y + ittrans.m22 * z + ittrans.m32 * w;
        //    var d = ittrans.m03 * x + ittrans.m13 * y + ittrans.m23 * z + ittrans.m33 * w;

        //    return new Plane(new Vector3(a, b, c), d);
        //}

        public static Matrix Scale(Vector3 vector)
        {
            Matrix m;
            m.m00 = vector.X; m.m01 = 0.0f; m.m02 = 0.0f; m.m03 = 0.0f;
            m.m10 = 0.0f; m.m11 = vector.Y; m.m12 = 0.0f; m.m13 = 0.0f;
            m.m20 = 0.0f; m.m21 = 0.0f; m.m22 = vector.Z; m.m23 = 0.0f;
            m.m30 = 0.0f; m.m31 = 0.0f; m.m32 = 0.0f; m.m33 = 1.0f;
            return m;
        }

        public static Matrix Translate(Vector3 vector)
        {
            Matrix m;
            m.m00 = 1.0f; m.m01 = 0.0f; m.m02 = 0.0f; m.m03 = vector.X;
            m.m10 = 0.0f; m.m11 = 1.0f; m.m12 = 0.0f; m.m13 = vector.Y;
            m.m20 = 0.0f; m.m21 = 0.0f; m.m22 = 1.0f; m.m23 = vector.Z;
            m.m30 = 0.0f; m.m31 = 0.0f; m.m32 = 0.0f; m.m33 = 1.0f;
            return m;
        }

        public static Matrix Rotate(Quaternion q)
        {
            // Precalculate coordinate products
            float x = q.X * 2.0F;
            float y = q.Y * 2.0F;
            float z = q.Z * 2.0F;
            float xx = q.X * x;
            float yy = q.Y * y;
            float zz = q.Z * z;
            float xy = q.X * y;
            float xz = q.X * z;
            float yz = q.Y * z;
            float wx = q.W * x;
            float wy = q.W * y;
            float wz = q.W * z;

            // Calculate 3x3 matrix from orthonormal basis
            Matrix m;
            m.m00 = 1.0f - (yy + zz); m.m10 = xy + wz; m.m20 = xz - wy; m.m30 = 0.0F;
            m.m01 = xy - wz; m.m11 = 1.0f - (xx + zz); m.m21 = yz + wx; m.m31 = 0.0F;
            m.m02 = xz + wy; m.m12 = yz - wx; m.m22 = 1.0f - (xx + yy); m.m32 = 0.0F;
            m.m03 = 0.0F; m.m13 = 0.0F; m.m23 = 0.0F; m.m33 = 1.0F;
            return m;
        }

        static readonly Matrix zeroMatrix = new Matrix(new Vector4(0, 0, 0, 0),
            new Vector4(0, 0, 0, 0),
            new Vector4(0, 0, 0, 0),
            new Vector4(0, 0, 0, 0));

        public static Matrix zero { get { return zeroMatrix; } }

        static readonly Matrix identityMatrix = new Matrix(new Vector4(1, 0, 0, 0),
            new Vector4(0, 1, 0, 0),
            new Vector4(0, 0, 1, 0),
            new Vector4(0, 0, 0, 1));

        public static Matrix identity { get { return identityMatrix; } }

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

            return string.Format("{0}\t{1}\t{2}\t{3}\n{4}\t{5}\t{6}\t{7}\n{8}\t{9}\t{10}\t{11}\n{12}\t{13}\t{14}\t{15}\n",
                m00.ToString(format, formatProvider), m01.ToString(format, formatProvider), m02.ToString(format, formatProvider), m03.ToString(format, formatProvider),
                m10.ToString(format, formatProvider), m11.ToString(format, formatProvider), m12.ToString(format, formatProvider), m13.ToString(format, formatProvider),
                m20.ToString(format, formatProvider), m21.ToString(format, formatProvider), m22.ToString(format, formatProvider), m23.ToString(format, formatProvider),
                m30.ToString(format, formatProvider), m31.ToString(format, formatProvider), m32.ToString(format, formatProvider), m33.ToString(format, formatProvider));
        }
    }
}

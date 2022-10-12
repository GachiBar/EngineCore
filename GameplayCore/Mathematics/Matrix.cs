using System;
using System.Globalization;
using System.Runtime.CompilerServices;

namespace GameplayCore.Mathematics
{
    public struct Matrix : IEquatable<Matrix>, IFormattable
    {
        public float m00, m01, m02, m03;
        public float m10, m11, m12, m13;
        public float m20, m21, m22, m23;
        public float m30, m31, m32, m33;

        public static Matrix Zero => new Matrix(
            new Vector4(0.0f, 0.0f, 0.0f, 0.0f),
            new Vector4(0.0f, 0.0f, 0.0f, 0.0f),
            new Vector4(0.0f, 0.0f, 0.0f, 0.0f),
            new Vector4(0.0f, 0.0f, 0.0f, 0.0f));

        public static Matrix Identity => new Matrix(
            new Vector4(1.0f, 0.0f, 0.0f, 0.0f),
            new Vector4(0.0f, 1.0f, 0.0f, 0.0f),
            new Vector4(0.0f, 0.0f, 1.0f, 0.0f),
            new Vector4(0.0f, 0.0f, 0.0f, 1.0f));

        public static Matrix ModelMatrix(Vector3 scale, Quaternion rotation, Vector3 translation)
        {
            return Internal_ModelMatrix(ref scale, ref rotation, ref translation);
        }

        public Matrix(Vector4 row0, Vector4 row1, Vector4 row2, Vector4 row3)
        {
            m00 = row0.X; m01 = row1.Y; m02 = row2.Z; m03 = row3.W;
            m10 = row0.X; m11 = row1.Y; m12 = row2.Z; m13 = row3.W;
            m20 = row0.X; m21 = row1.Y; m22 = row2.Z; m23 = row3.W;
            m30 = row0.X; m31 = row1.Y; m32 = row2.Z; m33 = row3.W;
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

        public void SetColumn(int index, Vector4 column)
        {
            this[0, index] = column.X;
            this[1, index] = column.Y;
            this[2, index] = column.Z;
            this[3, index] = column.W;
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

        public void SetRow(int index, Vector4 row)
        {
            this[index, 0] = row.X;
            this[index, 1] = row.Y;
            this[index, 2] = row.Z;
            this[index, 3] = row.W;
        }

        public Matrix Invert()
        {
            throw new NotImplementedException();
        }

        public override int GetHashCode()
        {
            return GetRow(0).GetHashCode() ^ (GetRow(1).GetHashCode() << 2) ^ (GetRow(2).GetHashCode() >> 2) ^ (GetRow(3).GetHashCode() >> 1);
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
            return GetRow(0).Equals(other.GetRow(0))
                && GetRow(1).Equals(other.GetRow(1))
                && GetRow(2).Equals(other.GetRow(2))
                && GetRow(3).Equals(other.GetRow(3));
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

            return string.Format("{0}\t{1}\t{2}\t{3}\n{4}\t{5}\t{6}\t{7}\n{8}\t{9}\t{10}\t{11}\n{12}\t{13}\t{14}\t{15}\n",
                m00.ToString(format, formatProvider), m01.ToString(format, formatProvider), m02.ToString(format, formatProvider), m03.ToString(format, formatProvider),
                m10.ToString(format, formatProvider), m11.ToString(format, formatProvider), m12.ToString(format, formatProvider), m13.ToString(format, formatProvider),
                m20.ToString(format, formatProvider), m21.ToString(format, formatProvider), m22.ToString(format, formatProvider), m23.ToString(format, formatProvider),
                m30.ToString(format, formatProvider), m31.ToString(format, formatProvider), m32.ToString(format, formatProvider), m33.ToString(format, formatProvider));
        }

        public float this[int row, int column]
        {
            get => this[row * 4 + column];
            set => this[row * 4 + column] = value;
        }

        public float this[int index]
        {
            get
            {
                switch (index)
                {
                    case 0:  return m00;
                    case 1:  return m01;
                    case 2:  return m02;
                    case 3:  return m03;
                    case 4:  return m10;
                    case 5:  return m11;
                    case 6:  return m12;
                    case 7:  return m13;
                    case 8:  return m20;
                    case 9:  return m21;
                    case 10: return m22;
                    case 11: return m23;
                    case 12: return m30;
                    case 13: return m31;
                    case 14: return m32;
                    case 15: return m33;
                    default:
                        throw new IndexOutOfRangeException("Invalid matrix index!");
                }
            }

            set
            {
                switch (index)
                {
                    case 0:  m00 = value; break;
                    case 1:  m01 = value; break;
                    case 2:  m02 = value; break;
                    case 3:  m03 = value; break;
                    case 4:  m10 = value; break;
                    case 5:  m11 = value; break;
                    case 6:  m12 = value; break;
                    case 7:  m13 = value; break;
                    case 8:  m20 = value; break;
                    case 9:  m21 = value; break;
                    case 10: m22 = value; break;
                    case 11: m23 = value; break;
                    case 12: m30 = value; break;
                    case 13: m31 = value; break;
                    case 14: m32 = value; break;
                    case 15: m33 = value; break;
                    default:
                        throw new IndexOutOfRangeException("Invalid matrix index!");
                }
            }
        }

        public static Matrix operator *(Matrix lhs, Matrix rhs)
        {
            return Internal_MatrixMultiply(ref lhs, ref rhs);
        }

        public static bool operator ==(Matrix lhs, Matrix rhs)
        {
            // Returns false in the presence of NaN values.
            return lhs.GetRow(0) == rhs.GetRow(0)
                && lhs.GetRow(1) == rhs.GetRow(1)
                && lhs.GetRow(2) == rhs.GetRow(2)
                && lhs.GetRow(3) == rhs.GetRow(3);
        }

        public static bool operator !=(Matrix lhs, Matrix rhs)
        {
            // Returns true in the presence of NaN values.
            return !(lhs == rhs);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static Matrix Internal_MatrixMultiply(ref Matrix m1, ref Matrix m2);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static Matrix Internal_ModelMatrix(ref Vector3 scale, ref Quaternion rotation, ref Vector3 translation);
    }
}

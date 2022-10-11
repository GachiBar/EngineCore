using System;

namespace GameplayCore.Mathematics
{
    public partial struct MathF
    {
        public const float PI = (float)Math.PI;

        public const float Infinity = float.PositiveInfinity;

        public const float NegativeInfinity = float.NegativeInfinity;

        public const float Deg2Rad = PI * 2.0f / 360.0f;

        public const float Rad2Deg = 1.0f / Deg2Rad;

        public static readonly float Epsilon = float.Epsilon;

        // We cannot round to more decimals than 15 according to docs for System.Math.Round.
        internal const int kMaxDecimals = 15;

        public static float Sin(float f)
        {
            return (float)Math.Sin(f);
        }

        public static float Cos(float f)
        {
            return (float)Math.Cos(f);
        }

        public static float Tan(float f)
        {
            return (float)Math.Tan(f);
        }

        public static float Asin(float f)
        {
            return (float)Math.Asin(f);
        }

        public static float Acos(float f)
        {
            return (float)Math.Acos(f);
        }

        public static float Atan(float f)
        {
            return (float)Math.Atan(f);
        }

        public static float Atan2(float y, float x)
        {
            return (float)Math.Atan2(y, x);
        }

        public static float Sqrt(float f)
        {
            return (float)Math.Sqrt(f);
        }

        public static float Abs(float f)
        {
            return Math.Abs(f);
        }

        public static int Abs(int value)
        {
            return Math.Abs(value);
        }

        public static float Min(float a, float b)
        {
            return a < b ? a : b;
        }

        public static float Min(params float[] values)
        {
            int len = values.Length;

            if (len == 0)
            {
                return 0;
            }

            float m = values[0];

            for (int i = 1; i < len; i++)
            {
                if (values[i] < m)
                {
                    m = values[i];
                }
            }

            return m;
        }

        public static int Min(int a, int b)
        {
            return a < b ? a : b;
        }

        public static int Min(params int[] values)
        {
            int len = values.Length;

            if (len == 0)
            {
                return 0;
            }

            int m = values[0];

            for (int i = 1; i < len; i++)
            {
                if (values[i] < m)
                {
                    m = values[i];
                }
            }

            return m;
        }

        public static float Max(float a, float b)
        {
            return a > b ? a : b;
        }

        public static float Max(params float[] values)
        {
            int len = values.Length;

            if (len == 0)
            {
                return 0;
            }

            float m = values[0];

            for (int i = 1; i < len; i++)
            {
                if (values[i] > m)
                {
                    m = values[i];
                }
            }

            return m;
        }

        public static int Max(int a, int b)
        {
            return a > b ? a : b;
        }

        public static int Max(params int[] values)
        {
            int len = values.Length;

            if (len == 0)
            {
                return 0;
            }

            int m = values[0];

            for (int i = 1; i < len; i++)
            {
                if (values[i] > m)
                {
                    m = values[i];
                }
            }

            return m;
        }

        public static float Pow(float f, float p)
        {
            return (float)Math.Pow(f, p);
        }

        public static float Exp(float power)
        {
            return (float)Math.Exp(power);
        }

        public static float Log(float f, float p)
        {
            return (float)Math.Log(f, p);
        }

        public static float Log(float f)
        {
            return (float)Math.Log(f);
        }

        public static float Log10(float f)
        {
            return (float)Math.Log10(f);
        }

        public static float Ceil(float f)
        {
            return (float)Math.Ceiling(f);
        }

        public static float Floor(float f)
        {
            return (float)Math.Floor(f);
        }

        public static float Round(float f)
        {
            return (float)Math.Round(f);
        }

        public static int CeilToInt(float f)
        {
            return (int)Math.Ceiling(f);
        }

        public static int FloorToInt(float f)
        {
            return (int)Math.Floor(f);
        }

        public static int RoundToInt(float f)
        {
            return (int)Math.Round(f);
        }

        public static float Sign(float f)
        {
            return f >= 0.0f ? 1.0f : -1.0f;
        }

        public static float Clamp(float value, float min, float max)
        {
            if (value < min)
            {
                value = min;
            }
            else if (value > max)
            {
                value = max;
            }

            return value;
        }

        public static int Clamp(int value, int min, int max)
        {
            if (value < min)
            {
                value = min;
            }
            else if (value > max)
            {
                value = max;
            }

            return value;
        }

        public static float Clamp01(float value)
        {
            if (value < 0.0f)
            {
                return 0.0f;
            }
            else if (value > 1.0f)
            {
                return 1.0f;
            }

            return value;
        }

        public static float Lerp(float a, float b, float t)
        {
            return a + (b - a) * Clamp01(t);
        }

        public static float LerpUnclamped(float a, float b, float t)
        {
            return a + (b - a) * t;
        }

        public static float LerpAngle(float a, float b, float t)
        {
            float delta = Repeat((b - a), 360);

            if (delta > 180)
            {
                delta -= 360;
            }

            return a + delta * Clamp01(t);
        }

        static public float MoveTowards(float current, float target, float maxDelta)
        {
            if (MathF.Abs(target - current) <= maxDelta)
            {
                return target;
            }

            return current + MathF.Sign(target - current) * maxDelta;
        }

        static public float MoveTowardsAngle(float current, float target, float maxDelta)
        {
            float deltaAngle = DeltaAngle(current, target);

            if (-maxDelta < deltaAngle && deltaAngle < maxDelta)
            {
                return target;
            }

            target = current + deltaAngle;
            return MoveTowards(current, target, maxDelta);
        }

        public static float SmoothStep(float from, float to, float t)
        {
            t = MathF.Clamp01(t);
            t = -2.0f * t * t * t + 3.0f * t * t;
            return to * t + from * (1.0f - t);
        }

        public static float Gamma(float value, float absmax, float gamma)
        {
            bool negative = value < 0.0f;
            float absval = Abs(value);

            if (absval > absmax)
            {
                return negative ? -absval : absval;
            }

            float result = Pow(absval / absmax, gamma) * absmax;
            return negative ? -result : result;
        }

        public static bool Approximately(float a, float b)
        {
            // If a or b is zero, compare that the other is less or equal to epsilon.
            // If neither a or b are 0, then find an epsilon that is good for
            // comparing numbers at the maximum magnitude of a and b.
            // Floating points have about 7 significant digits, so
            // 1.000001f can be represented while 1.0000001f is rounded to zero,
            // thus we could use an epsilon of 0.000001f for comparing values close to 1.
            // We multiply this epsilon by the biggest magnitude of a and b.
            return Abs(b - a) < Max(0.000001f * Max(Abs(a), Abs(b)), Epsilon * 8);
        }

        //public static float SmoothDamp(float current, float target, ref float currentVelocity, float smoothTime, float maxSpeed)
        //{
        //    float deltaTime = Time.deltaTime;
        //    return SmoothDamp(current, target, ref currentVelocity, smoothTime, maxSpeed, deltaTime);
        //}

        //public static float SmoothDamp(float current, float target, ref float currentVelocity, float smoothTime)
        //{
        //    float deltaTime = Time.deltaTime;
        //    float maxSpeed = Mathf.Infinity;
        //    return SmoothDamp(current, target, ref currentVelocity, smoothTime, maxSpeed, deltaTime);
        //}

        public static float SmoothDamp(float current, float target, ref float currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
        {
            // Based on Game Programming Gems 4 Chapter 1.10
            smoothTime = MathF.Max(0.0001f, smoothTime);
            float omega = 2.0f / smoothTime;

            float x = omega * deltaTime;
            float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
            float change = current - target;
            float originalTo = target;

            // Clamp maximum speed
            float maxChange = maxSpeed * smoothTime;
            change = MathF.Clamp(change, -maxChange, maxChange);
            target = current - change;

            float temp = (currentVelocity + omega * change) * deltaTime;
            currentVelocity = (currentVelocity - omega * temp) * exp;
            float output = target + (change + temp) * exp;

            // Prevent overshooting
            if (originalTo - current > 0.0f == output > originalTo)
            {
                output = originalTo;
                currentVelocity = (output - originalTo) / deltaTime;
            }

            return output;
        }

        //public static float SmoothDampAngle(float current, float target, ref float currentVelocity, float smoothTime, float maxSpeed)
        //{
        //    float deltaTime = Time.deltaTime;
        //    return SmoothDampAngle(current, target, ref currentVelocity, smoothTime, maxSpeed, deltaTime);
        //}

        //public static float SmoothDampAngle(float current, float target, ref float currentVelocity, float smoothTime)
        //{
        //    float deltaTime = Time.deltaTime;
        //    float maxSpeed = Mathf.Infinity;
        //    return SmoothDampAngle(current, target, ref currentVelocity, smoothTime, maxSpeed, deltaTime);
        //}

        public static float SmoothDampAngle(float current, float target, ref float currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
        {
            target = current + DeltaAngle(current, target);
            return SmoothDamp(current, target, ref currentVelocity, smoothTime, maxSpeed, deltaTime);
        }

        public static float Repeat(float t, float length)
        {
            return Clamp(t - MathF.Floor(t / length) * length, 0.0f, length);
        }

        public static float PingPong(float t, float length)
        {
            t = Repeat(t, length * 2.0f);
            return length - MathF.Abs(t - length);
        }

        public static float InverseLerp(float a, float b, float value)
        {
            if (a != b)
            {
                return Clamp01((value - a) / (b - a));
            }

            return 0.0f;
        }

        public static float DeltaAngle(float current, float target)
        {
            float delta = MathF.Repeat((target - current), 360.0f);

            if (delta > 180.0f)
            {
                delta -= 360.0f;
            }

            return delta;
        }

        internal static bool LineIntersection(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, ref Vector2 result)
        {
            float bx = p2.X - p1.X;
            float by = p2.Y - p1.Y;
            float dx = p4.X - p3.X;
            float dy = p4.Y - p3.Y;
            float bDotDPerp = bx * dy - by * dx;

            if (bDotDPerp == 0)
            {
                return false;
            }

            float cx = p3.X - p1.X;
            float cy = p3.Y - p1.Y;
            float t = (cx * dy - cy * dx) / bDotDPerp;

            result.X = p1.X + t * bx;
            result.Y = p1.Y + t * by;
            return true;
        }

        internal static bool LineSegmentIntersection(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, ref Vector2 result)
        {
            float bx = p2.X - p1.X;
            float by = p2.Y - p1.Y;
            float dx = p4.X - p3.X;
            float dy = p4.Y - p3.Y;
            float bDotDPerp = bx * dy - by * dx;

            if (bDotDPerp == 0)
            {
                return false;
            }

            float cx = p3.X - p1.X;
            float cy = p3.Y - p1.Y;
            float t = (cx * dy - cy * dx) / bDotDPerp;

            if (t < 0 || t > 1)
            {
                return false;
            }

            float u = (cx * by - cy * bx) / bDotDPerp;

            if (u < 0 || u > 1)
            {
                return false;
            }

            result.X = p1.X + t * bx;
            result.Y = p1.Y + t * by;
            return true;
        }

        static internal long RandomToLong(Random r)
        {
            var buffer = new byte[8];
            r.NextBytes(buffer);
            return (long)(BitConverter.ToUInt64(buffer, 0) & long.MaxValue);
        }

        internal static float ClampToFloat(double value)
        {
            if (double.IsPositiveInfinity(value))
            {
                return float.PositiveInfinity;
            }

            if (double.IsNegativeInfinity(value))
            {
                return float.NegativeInfinity;
            }

            if (value < float.MinValue)
            {
                return float.MinValue;
            }

            if (value > float.MaxValue)
            {
                return float.MaxValue;
            }

            return (float)value;
        }

        internal static int ClampToInt(long value)
        {
            if (value < int.MinValue)
            {
                return int.MinValue;
            }

            if (value > int.MaxValue)
            {
                return int.MaxValue;
            }

            return (int)value;
        }

        internal static float RoundToMultipleOf(float value, float roundingValue)
        {
            if (roundingValue == 0)
            {
                return value;
            }

            return MathF.Round(value / roundingValue) * roundingValue;
        }

        internal static float GetClosestPowerOfTen(float positiveNumber)
        {
            if (positiveNumber <= 0)
            {
                return 1;
            }

            return MathF.Pow(10, MathF.RoundToInt(MathF.Log10(positiveNumber)));
        }

        internal static int GetNumberOfDecimalsForMinimumDifference(float minDifference)
        {
            return MathF.Clamp(-MathF.FloorToInt(MathF.Log10(MathF.Abs(minDifference))), 0, kMaxDecimals);
        }

        internal static int GetNumberOfDecimalsForMinimumDifference(double minDifference)
        {
            return (int)Math.Max(0.0, -Math.Floor(Math.Log10(Math.Abs(minDifference))));
        }

        internal static float RoundBasedOnMinimumDifference(float valueToRound, float minDifference)
        {
            if (minDifference == 0)
            {
                return DiscardLeastSignificantDecimal(valueToRound);
            }

            return (float)Math.Round(valueToRound, GetNumberOfDecimalsForMinimumDifference(minDifference), MidpointRounding.AwayFromZero);
        }

        internal static double RoundBasedOnMinimumDifference(double valueToRound, double minDifference)
        {
            if (minDifference == 0)
            {
                return DiscardLeastSignificantDecimal(valueToRound);
            }

            return Math.Round(valueToRound, GetNumberOfDecimalsForMinimumDifference(minDifference), MidpointRounding.AwayFromZero);
        }

        internal static float DiscardLeastSignificantDecimal(float v)
        {
            int decimals = MathF.Clamp((int)(5 - MathF.Log10(MathF.Abs(v))), 0, kMaxDecimals);
            return (float)Math.Round(v, decimals, MidpointRounding.AwayFromZero);
        }

        internal static double DiscardLeastSignificantDecimal(double v)
        {
            int decimals = Math.Max(0, (int)(5 - Math.Log10(Math.Abs(v))));

            try
            {
                return Math.Round(v, decimals);
            }
            catch (ArgumentOutOfRangeException)
            {
                // This can happen for very small numbers.
                return 0;
            }
        }

    }
}

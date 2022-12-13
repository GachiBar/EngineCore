using GameplayCore.EngineApi;
using GameplayCore.Mathematics;
using System;

namespace GameplayCore
{
    public static class Gizmos
    {
        private const int Step = 10;
        private const int CircleStepsCount = 360 / Step + 1;
        private const int HalfCircleStepsCount = 180 / Step + 1;

        public static void DrawLine(Vector3 from, Vector3 to, Vector3 color)
        {
            DrawLine(Vector3.Zero, Quaternion.Identity, Vector3.One, from, to, color);
        }

        public static void DrawLine(Vector3 position, Quaternion rotation, Vector3 scale, Vector3 from, Vector3 to, Vector3 color)
        {
            var model = Matrix.Identity;
            model *= Matrix.Scaling(scale);
            model *= Matrix.RotationQuaternion(rotation);
            model *= Matrix.Translation(position);
            DrawLine(model, from, to, color);
        }

        public static void DrawLine(Matrix model, Vector3 from, Vector3 to, Vector3 color)
        {
            var points = new Vector3[] { from, to };
            DrawCurve(model, points, color);
        }

        public static void DrawCurve(Vector3[] points, Vector3 color)
        {
            DrawCurve(Vector3.Zero, Quaternion.Identity, Vector3.One, points, color);
        }

        public static void DrawCurve(Vector3 position, Quaternion rotation, Vector3 scale, Vector3[] points, Vector3 color)
        {
            var model = Matrix.Identity;
            model *= Matrix.Scaling(scale);
            model *= Matrix.RotationQuaternion(rotation);
            model *= Matrix.Translation(position);
            DrawCurve(model, points, color);
        }

        public static void DrawCurve(Matrix model, Vector3[] points, Vector3 color)
        {
            RenderApi.DrawCurve(points, color, model);
        }

        public static void DrawSphere(Vector3 position, float radius, Vector3 color)
        {
            DrawSphere(position, Quaternion.Identity, Vector3.One, radius, color);
        }

        public static void DrawSphere(Vector3 position, Quaternion rotation, Vector3 scale, float radius, Vector3 color)
        {
            var model = Matrix.Identity;
            model *= Matrix.Scaling(scale);
            model *= Matrix.RotationQuaternion(rotation);
            model *= Matrix.Translation(position);
            DrawSphere(model, radius, color);
        }

        public static void DrawSphere(Matrix model, float radius, Vector3 color)
        {
            var section1 = new Vector3[CircleStepsCount];

            for (int i = 0; i < CircleStepsCount; i++)
            {
                float angle = Step * i;
                float x = (float)Math.Cos(angle * MathUtil.Pi / 180) * radius;
                float y = (float)Math.Sin(angle * MathUtil.Pi / 180) * radius;
                Vector3 point = new Vector3(x, y, 0);
                section1[i] = point;
            }
           
            var section2 = new Vector3[CircleStepsCount];

            for (int i = 0; i < CircleStepsCount; i++)
            {
                float angle = Step * i;
                float x = (float)Math.Cos(angle * MathUtil.Pi / 180) * radius;
                float z = (float)Math.Sin(angle * MathUtil.Pi / 180) * radius;
                Vector3 point = new Vector3(x, 0, z);
                section2[i] = point;
            }
            
            var section3 = new Vector3[CircleStepsCount];

            for (int i = 0; i < CircleStepsCount; i++)
            {
                float angle = Step * i;
                float y = (float)Math.Cos(angle * MathUtil.Pi / 180) * radius;
                float z = (float)Math.Sin(angle * MathUtil.Pi / 180) * radius;
                Vector3 point = new Vector3(0, y, z);
                section3[i] = point;
            }

            DrawCurve(model, section1, color);
            DrawCurve(model, section2, color);
            DrawCurve(model, section3, color);
        }

        public static void DrawCube(Vector3 position, Vector3 size, Vector3 color)
        {
            DrawCube(position, Quaternion.Identity, Vector3.One, size, color);
        }

        public static void DrawCube(Vector3 position, Quaternion rotation, Vector3 scale, Vector3 size, Vector3 color)
        {
            var model = Matrix.Identity;
            model *= Matrix.Scaling(scale);
            model *= Matrix.RotationQuaternion(rotation);
            model *= Matrix.Translation(position);
            DrawCube(model, size, color);
        }

        public static void DrawCube(Matrix model, Vector3 size, Vector3 color)
        {
            Vector3 halfExtent = size / 2;

            Vector3[] frontFace =
            {
                new Vector3(-halfExtent.X,  halfExtent.Y, -halfExtent.Z),
                new Vector3(-halfExtent.X, -halfExtent.Y, -halfExtent.Z),
                new Vector3( halfExtent.X, -halfExtent.Y, -halfExtent.Z),
                new Vector3( halfExtent.X,  halfExtent.Y, -halfExtent.Z),
                new Vector3(-halfExtent.X,  halfExtent.Y, -halfExtent.Z),
            };            

            Vector3[] backFace =
            {
                new Vector3( halfExtent.X,  halfExtent.Y,  halfExtent.Z),
                new Vector3( halfExtent.X, -halfExtent.Y,  halfExtent.Z),
                new Vector3(-halfExtent.X, -halfExtent.Y,  halfExtent.Z),
                new Vector3(-halfExtent.X,  halfExtent.Y,  halfExtent.Z),
                new Vector3( halfExtent.X,  halfExtent.Y,  halfExtent.Z),
            };

            DrawCurve(model, frontFace, color);
            DrawCurve(model, backFace, color);

            DrawLine(
                model,
                new Vector3(-halfExtent.X, halfExtent.Y, -halfExtent.Z),
                new Vector3(-halfExtent.X, halfExtent.Y, halfExtent.Z),
                color);

            DrawLine(
                model,
                new Vector3(-halfExtent.X, -halfExtent.Y, -halfExtent.Z),
                new Vector3(-halfExtent.X, -halfExtent.Y, halfExtent.Z),
                color);

            DrawLine(
                model,
                new Vector3(halfExtent.X, halfExtent.Y, -halfExtent.Z),
                new Vector3(halfExtent.X, halfExtent.Y, halfExtent.Z),
                color);

            DrawLine(
                model,
                new Vector3(halfExtent.X, -halfExtent.Y, -halfExtent.Z),
                new Vector3(halfExtent.X, -halfExtent.Y, halfExtent.Z),
                color);
        }

        public static void DrawCapsule(Vector3 position, float height, float radius, Vector3 color)
        {
            DrawCapsule(position, Quaternion.Identity, Vector3.One, height, radius, color);
        }

        public static void DrawCapsule(Vector3 position, Quaternion rotation, Vector3 scale, float height, float radius, Vector3 color)
        {
            var model = Matrix.Identity;
            model *= Matrix.Scaling(scale);
            model *= Matrix.RotationQuaternion(rotation);
            model *= Matrix.Translation(position);
            DrawCapsule(model, height, radius, color);
        }

        public static void DrawCapsule(Matrix model, float height, float radius, Vector3 color)
        {
            float halfHeight = height / 2;
            
            Vector3[] section1 = new Vector3[2 * HalfCircleStepsCount + 1];
            int index1 = 0;

            for (; index1 < HalfCircleStepsCount; index1++)
            {
                float angle = Step * index1;
                float x = (float)Math.Cos(angle * MathUtil.Pi / 180) * radius;
                float y = (float)Math.Sin(angle * MathUtil.Pi / 180) * radius;
                Vector3 point = new Vector3(x, y + halfHeight, 0);
                section1[index1] = point;
            }

            for (; index1 < 2 * HalfCircleStepsCount; index1++)
            {
                float angle = Step * index1;
                float x = (float)Math.Cos(angle * MathUtil.Pi / 180) * radius;
                float y = (float)Math.Sin(angle * MathUtil.Pi / 180) * radius;
                Vector3 point = new Vector3(x, y - halfHeight, 0);
                section1[index1] = point;
            }

            section1[2 * HalfCircleStepsCount] = section1[0];

            Vector3[] section2 = new Vector3[2 * HalfCircleStepsCount + 1];
            int index2 = 0;

            for (; index2 < HalfCircleStepsCount; index2++)
            {
                float angle = Step * index2;
                float z = (float)Math.Cos(angle * MathUtil.Pi / 180) * radius;
                float y = (float)Math.Sin(angle * MathUtil.Pi / 180) * radius;
                Vector3 point = new Vector3(0, y + halfHeight, z);
                section2[index2] = point;
            }

            for (; index2 < 2 * HalfCircleStepsCount; index2++)
            {
                float angle = Step * index2;
                float z = (float)Math.Cos(angle * MathUtil.Pi / 180) * radius;
                float y = (float)Math.Sin(angle * MathUtil.Pi / 180) * radius;
                Vector3 point = new Vector3(0, y - halfHeight, z);
                section2[index2] = point;
            }

            section2[2 * HalfCircleStepsCount] = section2[0];

            var section3 = new Vector3[CircleStepsCount];

            for (int i = 0; i < CircleStepsCount; i++)
            {
                float angle = Step * i;
                float x = (float)Math.Cos(angle * MathUtil.Pi / 180) * radius;
                float z = (float)Math.Sin(angle * MathUtil.Pi / 180) * radius;
                Vector3 point = new Vector3(x, halfHeight, z);
                section3[i] = point;
            }

            var section4 = new Vector3[CircleStepsCount];

            for (int i = 0; i < CircleStepsCount; i++)
            {
                float angle = Step * i;
                float x = (float)Math.Cos(angle * MathUtil.Pi / 180) * radius;
                float z = (float)Math.Sin(angle * MathUtil.Pi / 180) * radius;
                Vector3 point = new Vector3(x, -halfHeight, z);
                section4[i] = point;
            }

            DrawCurve(model, section1, color);
            DrawCurve(model, section2, color);
            DrawCurve(model, section3, color);
            DrawCurve(model, section4, color);
        }
    }
}

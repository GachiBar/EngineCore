using GameplayCore.EngineApi;
using GameplayCore.Mathematics;
using System;

namespace GameplayCore
{
    public static class Gizmos
    {
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
            int circle = 360;
            int step = 10;
            int stepCount = circle / step + 2;

            var circle1 = new Vector3[stepCount];

            for (int i = 0; i < stepCount; i++)
            {
                float angle = step * i;
                float x = (float)Math.Cos(angle * MathUtil.Pi / 180) * radius;
                float y = (float)Math.Sin(angle * MathUtil.Pi / 180) * radius;
                Vector3 point = new Vector3(x, y, 0);
                circle1[i] = point;
            }

            DrawCurve(model, circle1, color);
            var circle2 = new Vector3[stepCount];

            for (int i = 0; i < stepCount; i++)
            {
                float angle = step * i;
                float x = (float)Math.Cos(angle * MathUtil.Pi / 180) * radius;
                float z = (float)Math.Sin(angle * MathUtil.Pi / 180) * radius;
                Vector3 point = new Vector3(x, 0, z);
                circle2[i] = point;
            }

            DrawCurve(model, circle2, color);
            var circle3 = new Vector3[stepCount];

            for (int i = 0; i < stepCount; i++)
            {
                float angle = step * i;
                float y = (float)Math.Cos(angle * MathUtil.Pi / 180) * radius;
                float z = (float)Math.Sin(angle * MathUtil.Pi / 180) * radius;
                Vector3 point = new Vector3(0, y, z);
                circle3[i] = point;
            }

            DrawCurve(model, circle3, color);
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

            DrawCurve(model, frontFace, color);

            Vector3[] backFace =
            {
                new Vector3( halfExtent.X,  halfExtent.Y,  halfExtent.Z),
                new Vector3( halfExtent.X, -halfExtent.Y,  halfExtent.Z),
                new Vector3(-halfExtent.X, -halfExtent.Y,  halfExtent.Z),
                new Vector3(-halfExtent.X,  halfExtent.Y,  halfExtent.Z),
                new Vector3( halfExtent.X,  halfExtent.Y,  halfExtent.Z),
            };

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
    }
}

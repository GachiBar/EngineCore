using GameplayCore.EngineApi;
using GameplayCore.Mathematics;

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

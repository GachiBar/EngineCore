using GameplayCore.Mathematics;
using System.Runtime.CompilerServices;

namespace GameplayCore.EngineApi
{
    internal static class RenderApi
    {
        private static unsafe void* Renderer { get; set; }

        public static void RegisterModel(ulong id)
        {
            unsafe
            {
                Internal_RegisterModel(Renderer, id, "");
            }           
        }

        public static void DrawModel(ulong id, float metallic, float roughness, Matrix modelMatrix)
        {
            unsafe
            {
                Internal_DrawModel(Renderer, id, metallic, roughness, modelMatrix);
            }            
        }

        public static void DrawDirectionalLight(Vector3 direction, float itencity, Vector3 color)
        {
            unsafe
            {
                Vector4 inColor = new Vector4(color, 0.0f);
                Internal_DrawDirectionalLight(Renderer, direction, itencity, inColor);
            }
        }

        public static void DrawCurve(Vector3[] points, Vector3 color, Matrix modelMatrix)
        {
            unsafe
            {
                Internal_DrawCurve(Renderer, points, color, modelMatrix);
            }
        }

        public static void SetViewProjection(float ellapsed, Matrix view, Matrix projection)
        {
            unsafe
            {
                Internal_SetViewProjection(Renderer, ellapsed, view, projection);
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_RegisterModel(void* renderer, ulong id, string path);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_DrawModel(void* renderer, ulong id, float metallic, float roughness, Matrix modelMatrix);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_DrawDirectionalLight(void* renderer, Vector3 direction, float itencity, Vector4 color);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_DrawCurve(void* renderer, Vector3[] points, Vector3 color, Matrix modelMatrix);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_SetViewProjection(void* renderer, float ellapsed, Matrix view, Matrix projection);
    }
}

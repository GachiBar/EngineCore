using System;
using GameplayCore.Mathematics;
using System.Runtime.CompilerServices;
using GameplayCore.Resources;

namespace GameplayCore.EngineApi
{
    internal static class RenderApi
    {
        private static unsafe void* Renderer { get; set; }
        private static ulong _lastTextureId = 1;
        private static ulong _lastMeshId = 1;

        public static void RegisterModel(ulong id, string path="")
        {
            unsafe
            {
                Internal_RegisterModel(Renderer, id, path);
            }           
        }
        
        public static void RegisterTexture(ulong id, string path="")
        {
            unsafe
            {
                Internal_RegisterTexture(Renderer, id, path);
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

        public static bool IsIdUsed(ulong id)
        {
            bool value;
            unsafe
            {
                value = Internal_IsIdUsed(Renderer, id);
            }
            return value;
        }

        public static bool IsMeshIdUsed(ulong id)
        {
            bool value;
            unsafe
            {
                value = Internal_IsMeshIdUsed(Renderer, id);
            }
            return value;
        }
        
        public static bool IsTextureIdUsed(ulong id)
        {
            bool value;
            unsafe
            {
                value = Internal_IsTextureIdUsed(Renderer, id);
            }
            return value;
        }

        public static ulong CalculateFreeTextureId()
        {
            while (IsIdUsed(_lastTextureId))
                _lastTextureId++;

            return _lastTextureId;
        }
        
        public static ulong CalculateFreeMeshId()
        {
            while (IsIdUsed(_lastMeshId))
                _lastMeshId++;

            return _lastMeshId;
        }

        public static ulong CalculateFreeId()
        {
            CalculateFreeTextureId();
            CalculateFreeMeshId();

            return System.Math.Max(_lastTextureId, _lastMeshId);
        }

        public static Internal_Material PullMaterial()
        {
            Internal_Material material = new Internal_Material();
            try
            {
                material = Internal_PullMaterial(0);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
                throw;
            }
             
            material.DebugOutput();
            return material;
        }

        public static void CommitMaterial(Internal_Material material)
        {
            try
            {
                Internal_CommitMaterial(material);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
                throw;
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_RegisterModel(void* renderer, ulong id, string path);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_RegisterTexture(void* renderer, ulong id, string path);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_DrawModel(void* renderer, ulong id, float metallic, float roughness, Matrix modelMatrix);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_DrawDirectionalLight(void* renderer, Vector3 direction, float itencity, Vector4 color);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_DrawCurve(void* renderer, Vector3[] points, Vector3 color, Matrix modelMatrix);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_SetViewProjection(void* renderer, float ellapsed, Matrix view, Matrix projection);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe bool Internal_IsIdUsed(void* renderer, ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe bool Internal_IsMeshIdUsed(void* renderer, ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe bool Internal_IsTextureIdUsed(void* renderer, ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe Internal_Material Internal_PullMaterial(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_CommitMaterial(Internal_Material material);
    }
}

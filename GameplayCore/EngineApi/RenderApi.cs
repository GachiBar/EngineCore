﻿using GameplayCore.Mathematics;
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

        public static void DrawModel(ulong id, Matrix modelMatrix)
        {
            unsafe
            {
                Internal_DrawModel(Renderer, id, modelMatrix);
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
        extern private static unsafe void Internal_DrawModel(void* renderer, ulong id, Matrix modelMatrix);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_SetViewProjection(void* renderer, float ellapsed, Matrix view, Matrix projection);
    }
}
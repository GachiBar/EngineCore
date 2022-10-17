using GameplayCore.Mathematics;
using System.Runtime.CompilerServices;

namespace GameplayCore.EngineApi
{
    public static class Render
    {
        private static unsafe void* Renderer { get; set; }

        public static void RegisterModel(ulong id)
        {
            unsafe
            {
                Internal_RegisterModel(Renderer, id);
            }           
        }

        public static void DrawModel(ulong id, Matrix modelMatrix)
        {
            unsafe
            {
                Internal_DrawModel(Renderer, id, modelMatrix);
            }            
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_RegisterModel(void* renderer, ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static unsafe void Internal_DrawModel(void* renderer, ulong id, Matrix modelMatrix);
    }
}

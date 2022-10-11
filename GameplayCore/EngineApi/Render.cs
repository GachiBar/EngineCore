using System.Runtime.CompilerServices;

namespace GameplayCore.EngineApi
{
    public static class Render
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static void Internal_RegisterModel(ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static void Internal_DrawModel(ulong id);
    }
}

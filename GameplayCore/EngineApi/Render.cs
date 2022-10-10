using System.Runtime.CompilerServices;

namespace GameplayCore.EngineApi
{
    public static class Render
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static void RegisterModel(ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static void DrawModel(ulong id);
    }
}

using System.Runtime.CompilerServices;

namespace GameplayCore
{
    public static class Renderer
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static void RegisterModel(ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static void DrawModel(ulong id);
    }
}

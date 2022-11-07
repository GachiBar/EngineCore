using GameplayCore.Mathematics;
using System.Runtime.CompilerServices;

namespace GameplayCore
{
    public static class Input
    {
        public static Vector2 MousePosition { get; private set; }

        public static bool IsPressed(string keyName)
        {
            return Internal_IsPressed(keyName);
        }

        public static bool WasJustPressed(string keyName)
        {
            return Internal_WasJustPressed(keyName);
        }

        public static bool WasJustReleased(string keyName)
        {
            return Internal_WasJustReleased(keyName);
        }

        public static bool IsActionPressed(string keyName)
        {
            return Internal_IsActionPressed(keyName);
        }

        public static bool WasActionJustPressed(string keyName)
        {
            return Internal_WasActionJustPressed(keyName);
        }

        public static bool WasActionJustReleased(string keyName)
        {
            return Internal_WasActionJustReleased(keyName);
        }

        public static float GetKeyValue(string keyName)
        {
            return Internal_GetKeyValue(keyName);
        }

        public static float GetAxisValue(string axis)
        {
            return Internal_GetAxisValue(axis);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static bool Internal_IsPressed(string keyName);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static bool Internal_WasJustPressed(string keyName);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static bool Internal_WasJustReleased(string keyName);


        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static bool Internal_IsActionPressed(string keyName);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static bool Internal_WasActionJustPressed(string keyName);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static bool Internal_WasActionJustReleased(string keyName);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static float Internal_GetKeyValue(string keyName);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static float Internal_GetAxisValue(string axis_name);
    }
}

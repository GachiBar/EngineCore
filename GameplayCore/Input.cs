using System;
using System.Runtime.CompilerServices;
using GameplayCore.Mathematics;

namespace GameplayCore
{
    public static class Input
    {
        private static string[] _keysNames;

        static Input()
        {
            _keysNames = Enum.GetNames(typeof(Keys));
        }

        public static Vector2 MousePosition { get; private set; }

        public static bool IsPressed(Keys key)
        {
            var keyName = _keysNames[(int)key];
            return IsPressed(keyName);
        }

        public static bool IsPressed(string keyName)
        {
            return Internal_IsPressed(keyName);
        }

        public static bool WasJustPressed(Keys key)
        {
            var keyName = _keysNames[(int)key];
            return WasJustPressed(keyName);
        }

        public static bool WasJustPressed(string keyName)
        {
            return Internal_WasJustPressed(keyName);
        }

        public static bool WasJustReleased(Keys key)
        {
            var keyName = _keysNames[(int)key];
            return WasJustReleased(keyName);
        }

        public static bool WasJustReleased(string keyName)
        {
            return Internal_WasJustReleased(keyName);
        }

        public static bool IsActionPressed(string actionName)
        {
            return Internal_IsActionPressed(actionName);
        }

        public static bool WasActionJustPressed(string actionName)
        {
            return Internal_WasActionJustPressed(actionName);
        }

        public static bool WasActionJustReleased(string actionName)
        {
            return Internal_WasActionJustReleased(actionName);
        }

        public static float GetKeyValue(string keyName)
        {
            return Internal_GetKeyValue(keyName);
        }

        public static float GetKeyValue(Keys key)
        {
            var keyName = _keysNames[(int)key];
            return Internal_GetKeyValue(keyName);
        }

        public static float GetAxisValue(string axisName)
        {
            return Internal_GetAxisValue(axisName);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static bool Internal_IsPressed(string keyName);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static bool Internal_WasJustPressed(string keyName);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static bool Internal_WasJustReleased(string keyName);


        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static bool Internal_IsActionPressed(string actionName);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static bool Internal_WasActionJustPressed(string actionName);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static bool Internal_WasActionJustReleased(string actonName);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static float Internal_GetKeyValue(string keyName);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static float Internal_GetAxisValue(string axisName);
    }
}

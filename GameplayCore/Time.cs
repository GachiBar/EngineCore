namespace GameplayCore
{
    public static class Time
    {
        public static float FixedDeltaTime { get; private set; } = 16.0f / 1000;
        public static float DeltaTime { get; private set; }
        public static float EllapsedTime { get; private set; }
    }
}

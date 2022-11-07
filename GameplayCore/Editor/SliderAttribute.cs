using System;

namespace GameplayCore.Editor
{
    public class SliderAttribute : Attribute
    {
        public float Min { get; private set; }
        public float Max { get; private set; }

        public SliderAttribute(float min, float max)
        {
            Min = min;
            Max = max;
        }
    }
}

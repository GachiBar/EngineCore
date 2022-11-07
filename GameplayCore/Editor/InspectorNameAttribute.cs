using System;

namespace GameplayCore.Editor
{
    public class InspectorNameAttribute : Attribute
    {
        public string Name { get; private set; }

        public InspectorNameAttribute(string name)
        {
            Name = name;
        }
    }
}

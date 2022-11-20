using System;
using System.Collections.Generic;
using System.Reflection;

namespace GameplayCore.Serialization
{
    public static class ReflectionExtensions
    {
        public static IEnumerable<FieldInfo> FilterFields(FieldInfo[] fields, Func<FieldInfo, bool> filter)
        {
            foreach (var field in fields)
            {
                if (filter(field))
                    yield return field;
            }
        }

        public static bool HasAttribute(FieldInfo fieldInfo, Type type)
        {
            foreach (var attribute in fieldInfo.CustomAttributes)
            {
                if (attribute.AttributeType == type)
                    return true;
            }

            return false;
        }
        
        public static IEnumerable<FieldInfo> SerializableFieldsIterator(Object item)
        {
            FieldInfo[] fields = item
                .GetType()
                .GetFields(BindingFlags.Public | BindingFlags.Instance);

            foreach (var field in ReflectionExtensions.FilterFields(fields, info
                         => !ReflectionExtensions.HasAttribute(info, typeof(HideInInspectorAttribute))))
            {
                yield return field;
            }

            fields = item
                .GetType()
                .GetFields(BindingFlags.NonPublic | BindingFlags.Instance);

            foreach (var field in ReflectionExtensions.FilterFields(fields, info
                         => ReflectionExtensions.HasAttribute(info, typeof(SerializeFieldAttribute))))
            {
                yield return field;
            }
        }

        public static void FindSetFieldValue(object item, string fieldName, object value)
        {
            FieldInfo field = item.GetType().GetField(fieldName);
            field.SetValue(item, value);
        }
    }
}
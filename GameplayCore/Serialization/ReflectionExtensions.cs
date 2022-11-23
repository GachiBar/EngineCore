using System;
using System.Collections.Generic;
using System.Linq;
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
                .GetFieldsRecursive(BindingFlags.NonPublic | BindingFlags.Instance);

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
        
        public static FieldInfo[] GetFieldsRecursive(this Type type, BindingFlags bindingFlags)
        {
            FieldInfo[] fieldInfos = type.GetFields(bindingFlags);

            // If this class doesn't have a base, don't waste any time
            if (type.BaseType == typeof(object))
            {
                return fieldInfos;
            }
            else
            {   // Otherwise, collect all types up to the furthest base class
                var currentType = type;
                var fieldComparer = new FieldInfoComparer();
                var fieldInfoList = new HashSet<FieldInfo>(fieldInfos, fieldComparer);
                while (currentType != typeof(object))
                {
                    fieldInfos = currentType.GetFields(bindingFlags);
                    fieldInfoList.UnionWith(fieldInfos);
                    currentType = currentType.BaseType;
                }
                return fieldInfoList.ToArray();
            }
        }

        private class FieldInfoComparer : IEqualityComparer<FieldInfo>
        {
            public bool Equals(FieldInfo x, FieldInfo y)
            {
                return x.DeclaringType == y.DeclaringType && x.Name == y.Name;
            }

            public int GetHashCode(FieldInfo obj)
            {
                return obj.Name.GetHashCode() ^ obj.DeclaringType.GetHashCode();
            }
        }
    }
}
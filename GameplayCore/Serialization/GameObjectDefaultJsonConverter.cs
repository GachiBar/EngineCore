using System;
using System.Collections.Generic;
using System.Reflection;
using Newtonsoft.Json;

namespace GameplayCore.Serialization
{
    public class GameObjectDefaultJsonConverter : JsonConverter<GameObject>
    {
        private IEnumerable<FieldInfo> FilterFields(FieldInfo[] fields, Func<FieldInfo, bool> filter)
        {
            foreach (var field in fields)
            {
                if (filter(field))
                    yield return field;
            }
        }

        private bool HasAttribute(FieldInfo fieldInfo, Type type)
        {
            foreach (var attribute in fieldInfo.CustomAttributes)
            {
                if (attribute.AttributeType == type)
                    return true;
            }

            return false;
        }

        public override void WriteJson(JsonWriter writer, GameObject gameObject, JsonSerializer serializer)
        {
            writer.WriteStartObject();
            
            writer.WritePropertyName("Guid");
            writer.WriteValue(gameObject.Guid.ToString());
            
            writer.WritePropertyName("Components");
            writer.WriteStartArray();

            foreach (var component in gameObject)
            {
                writer.WriteStartObject();
                writer.WritePropertyName("Type");
                writer.WriteValue(component.GetType().ToString());
                FieldInfo[] publicFields = component.GetType().GetFields(BindingFlags.Public | BindingFlags.Instance);
                foreach (var field in FilterFields(publicFields,
                             info => !HasAttribute(info, typeof(HideInInspectorAttribute))))
                {
                    writer.WritePropertyName(field.Name);
                    serializer.Serialize(writer, field.GetValue(component));
                }

                FieldInfo[] privateFields =
                    component.GetType().GetFields(BindingFlags.NonPublic | BindingFlags.Instance);
                foreach (var field in FilterFields(privateFields,
                             info => HasAttribute(info, typeof(SerializeFieldAttribute))))
                {
                    writer.WritePropertyName(field.Name);
                    serializer.Serialize(writer, field.GetValue(component));
                }

                writer.WriteEndObject();
            }

            writer.WriteEndArray();
            writer.WriteEndObject();
        }

        public override GameObject ReadJson(JsonReader reader, Type objectType, GameObject existingValue,
            bool hasExistingValue,
            JsonSerializer serializer)
        {
            throw new NotImplementedException();
        }
    }
}
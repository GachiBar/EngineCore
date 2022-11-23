using System;
using System.Reflection;
using GameplayCore.Resources;
using Newtonsoft.Json;

namespace GameplayCore.Serialization
{
    public class ResourceJsonConverter: JsonConverter<Resource>
    {
        public override void WriteJson(JsonWriter writer, Resource resource, JsonSerializer serializer)
        {
            writer.WriteStartObject();
            
            JsonExtensions.WriteStringProperty(writer, "Type", resource.GetType().ToString());
            // JsonExtensions.WriteStringProperty(writer, "Guid", resource.Guid.ToString());

            foreach (var field in ReflectionExtensions.SerializableFieldsIterator(resource))
            {
                if (typeof(GameObject).IsAssignableFrom(field.FieldType)
                    || typeof(Scene).IsAssignableFrom(field.FieldType)) continue;
                
                if (typeof(Resource).IsAssignableFrom(field.FieldType))
                {
                    System.Guid? guid = (field.GetValue(resource) as Resource)?.Guid;
                    JsonExtensions.WriteSerializedProperty(writer, serializer, field.Name, guid);
                    continue;
                }
                
                JsonExtensions.WriteSerializedProperty(writer, serializer, field.Name, field.GetValue(resource));
            }
            
            writer.WriteEndObject();
        }

        public override Resource ReadJson(JsonReader reader, Type objectType, Resource existingValue, bool hasExistingValue,
            JsonSerializer serializer)
        {
            if (reader.TokenType != JsonToken.StartObject)
                throw new JsonException("Can't get start of object!");
            
            string typeName = JsonExtensions.ReadString(reader, "Type");
            Type resourceType = Type.GetType(typeName);
            Resource current = Activator.CreateInstance(resourceType) as Resource;

            if (current == null) return null;
            
            // string guid = JsonExtensions.ReadString(reader, "Guid");
            // ReflectionExtensions.FindSetFieldValue( current, "Guid", Guid.Parse(guid));
            
            while (reader.Read())
            {
                if (!JsonExtensions.CheckToken(reader, JsonToken.PropertyName))
                    continue;
                
                string fieldName = JsonExtensions.ReadString(reader);
                FieldInfo field = resourceType.GetField(fieldName);
                object fieldValue = serializer.Deserialize(reader, field.FieldType);
                field.SetValue(current, fieldValue);
            }

            if (reader.TokenType != JsonToken.EndObject)
                throw new JsonException("Can't get end of object!");
            
            return current;
        }
    }
}
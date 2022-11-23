using Newtonsoft.Json;

namespace GameplayCore.Serialization
{
    public static class JsonExtensions
    {
        public static bool CheckStartObject(JsonReader reader)
        {
            return CheckToken(reader, JsonToken.StartObject);
        }

        public static bool CheckEndObject(JsonReader reader)
        {
            return CheckToken(reader, JsonToken.EndObject);
        }

        public static bool CheckToken(JsonReader reader, JsonToken tokenType)
        {
            reader.Read();
            return reader.TokenType == tokenType;
        }
        
        public static string ReadString(JsonReader reader)
        {
            reader.Read();
            return reader.Value as string;
        }
        
        public static string ReadString(JsonReader reader, string propertyName)
        {
            ReadCheckPropertyName(reader, propertyName);
            return ReadString(reader);
        }

        public static void WriteStringProperty(JsonWriter writer, string propertyName, string value)
        {
            writer.WritePropertyName(propertyName);
            writer.WriteValue(value);
        }
        
        public static void WriteSerializedProperty(JsonWriter writer, JsonSerializer serializer, string propertyName, object value)
        {
            writer.WritePropertyName(propertyName);
            serializer.Serialize(writer, value);
        }

        public static bool ReadCheckPropertyName(JsonReader reader, string propertyName)
        {
            reader.Read();
            if (reader.TokenType != JsonToken.PropertyName)
                return false;

            string name = reader.Value as string;
            if (propertyName != name)
                return false;

            return true;
        }
    }
}
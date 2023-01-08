using GameplayCore.AI;
using GameplayCore.Resources;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace GameplayCore.Serialization
{
    public class AIActionsListJsonConverter : JsonConverter<List<AIAction>>
    {
        public override List<AIAction> ReadJson(JsonReader reader, Type objectType, List<AIAction> existingValue, bool hasExistingValue, JsonSerializer serializer)
        {
            var result = new List<AIAction>();

            if (reader.TokenType != JsonToken.StartObject)
            {
                return null;
            }
            reader.Read();

            if (reader.TokenType != JsonToken.PropertyName)
            {
                return null;
            }
            if (reader.Value as string != "Actions")
            {
                return null;
            }
            reader.Read();

            if (reader.TokenType != JsonToken.StartArray)
            {
                return null;
            }
            reader.Read();

            while (reader.TokenType != JsonToken.EndArray)
            {
                var typeName = reader.Value as string;
                var type = Type.GetType(typeName);
                var action = (AIAction)Activator.CreateInstance(type);
                result.Add(action);
                reader.Read();
            }
            reader.Read();

            if (reader.TokenType != JsonToken.EndObject)
            {
                return null;
            }

            return result;
        }

        public override void WriteJson(JsonWriter writer, List<AIAction> value, JsonSerializer serializer)
        {
            writer.WriteStartObject();
            writer.WritePropertyName("Actions");
            writer.WriteStartArray();

            foreach (var action in value)
            {
                var type = action.GetType();
                writer.WriteValue(type.ToString());
            }

            writer.WriteEndArray();
            writer.WriteEndObject();
        }
    }
}

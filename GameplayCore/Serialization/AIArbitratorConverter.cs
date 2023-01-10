using GameplayCore.AI;
using Newtonsoft.Json;
using System;

namespace GameplayCore.Serialization
{
    public class AIArbitratorConverter : JsonConverter<AIArbitrator>
    {
        public override AIArbitrator ReadJson(
            JsonReader reader, 
            Type objectType, 
            AIArbitrator existingValue, 
            bool hasExistingValue, 
            JsonSerializer serializer)
        {
            if (reader.Value == null)
            {
                return null;
            }

            var arbitratorType = Type.GetType(reader.Value.ToString());
            return (AIArbitrator)Activator.CreateInstance(arbitratorType);
        }

        public override void WriteJson(
            JsonWriter writer, 
            AIArbitrator value, 
            JsonSerializer serializer)
        {
            if (value == null)
            {
                writer.WriteNull();
            }

            var arbitratorTypeName = value.GetType().ToString();
            writer.WriteValue(arbitratorTypeName);
        }
    }
}

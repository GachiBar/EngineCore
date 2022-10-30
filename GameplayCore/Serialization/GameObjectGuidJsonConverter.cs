using System;
using Newtonsoft.Json;

namespace GameplayCore.Serialization
{
    public class GameObjectGuidJsonConverter: JsonConverter<GameObject>
    {
        public override void WriteJson(JsonWriter writer, GameObject gameObject, JsonSerializer serializer) =>
            writer.WriteValue(gameObject.Guid.ToString());

        public override GameObject ReadJson(JsonReader reader, Type objectType, GameObject existingValue, bool hasExistingValue,
            JsonSerializer serializer) => new GameObject(null, System.Guid.Parse(reader.Value.ToString()));
    }    
}


using System;
using GameplayCore.Resources;
using Newtonsoft.Json;

namespace GameplayCore.Serialization
{
    public class ResourceGuidJsonConverter : JsonConverter<Resource>
    {
        public override void WriteJson(JsonWriter writer, Resource value, JsonSerializer serializer)
            => writer.WriteValue(value.Guid.ToString());

        public override Resource ReadJson(JsonReader reader, Type objectType, Resource existingValue,
            bool hasExistingValue,
            JsonSerializer serializer)
            => MetadataReader.TryGetByGuid(reader.Value != null ? System.Guid.Parse(reader.Value.ToString()):System.Guid.Empty);
    }
}
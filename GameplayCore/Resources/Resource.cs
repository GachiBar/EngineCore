using System;
using System.IO;
using GameplayCore.Serialization;
using Newtonsoft.Json;

namespace GameplayCore.Resources
{
    public class Resource
    {
        [SerializeField] private System.Guid _guid;
        private string _filepath;
        
        public System.Guid Guid => _guid;
        public string FilePath => _filepath;

        public Resource(string filepath, System.Guid? guid = null)
        {
            _filepath = filepath;
            
            if (guid == null)
                guid = System.Guid.NewGuid();

            _guid = guid.Value;
        }

        public virtual void CreateMetafile(string path)
        {
            FileStream file = File.Create(path);
            JsonTextWriter writer = new JsonTextWriter(new StreamWriter(file));
            JsonSerializer serializer = JsonSerializer.CreateDefault();
            
            writer.Formatting = Formatting.Indented;

            writer.WriteStartObject();
            writer.WritePropertyName("Type");
            writer.WriteValue(GetType().ToString());
            writer.WritePropertyName("Guid");
            writer.WriteValue(_guid.ToString());
            writer.WriteEndObject();
            
            file.Close();
        }

        public virtual void ReadMetafile(string path)
        {
            FileStream file = File.OpenRead(path);
            JsonTextReader reader = new JsonTextReader(new StreamReader(file));
            
            JsonExtensions.CheckStartObject(reader);
            
            JsonExtensions.ReadCheckPropertyName(reader, "Type");
            string typeName = JsonExtensions.ReadString(reader);
            
            JsonExtensions.ReadCheckPropertyName(reader, "Guid");
            string guidString = JsonExtensions.ReadString(reader);
            _guid = Guid.Parse(guidString);
            
            JsonExtensions.CheckEndObject(reader);
            
            file.Close();
        }
    }
}
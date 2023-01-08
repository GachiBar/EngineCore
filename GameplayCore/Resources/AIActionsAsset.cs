using GameplayCore.AI;
using GameplayCore.Serialization;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;

namespace GameplayCore.Resources
{
    public class AIActionsAsset : Resource, ILoadable
    {
        private List<AIAction> _actions;
        private JsonSerializerSettings _options;

        public IReadOnlyList<AIAction> Actions => _actions;

        public AIActionsAsset(string filepath, Guid? guid = null) 
            : base(filepath, guid)
        {
            _options = new JsonSerializerSettings()
            {
                Formatting = Formatting.Indented,
                Converters = { new AIActionsListJsonConverter() }
            };
        }

        public void Load()
        {
            using (StreamReader reader = new StreamReader(FilePath))
            {
                var data = reader.ReadToEnd();
                _actions = JsonConvert.DeserializeObject<List<AIAction>>(data, _options);
            }
        }

        public void Save()
        {            
            using (var writer = new StreamWriter(FilePath))
            {
                var json = JsonConvert.SerializeObject(_actions, _options);
                writer.Write(json);
            }
        }
    }
}

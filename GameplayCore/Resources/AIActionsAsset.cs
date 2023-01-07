using GameplayCore.AI;
using GameplayCore.Serialization;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameplayCore.Resources
{
    public class AIActionsAsset : Resource, ILoadable
    {
        private AIActionsList _actions;

        public IReadOnlyList<AIAction> Actions => _actions.Actions;

        public AIActionsAsset(string filepath, Guid? guid = null) 
            : base(filepath, guid)
        { }

        public void Load()
        {
            using (StreamReader reader = new StreamReader(FilePath))
            {
                var data = reader.ReadToEnd();

                JsonSerializerSettings options = new JsonSerializerSettings()
                {
                    Formatting = Formatting.Indented,
                    Converters = { new AIActionsListJsonConverter() }
                };

                _actions = JsonConvert.DeserializeObject<AIActionsList>(data, options);
            }
        }
    }
}

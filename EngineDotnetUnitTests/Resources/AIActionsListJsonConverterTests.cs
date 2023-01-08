using GameplayCore.AI;
using GameplayCore.AI.Enemy.Actions;
using GameplayCore.Serialization;
using Newtonsoft.Json;
using NUnit.Framework;
using System.Collections.Generic;
using System.Linq;

namespace EngineDotnetUnitTests.Resources
{
    public class AIActionsListJsonConverterTests
    {
        [Test]
        [TestCaseSource(nameof(ActionsGenerator))]
        public void ReadJson_CorrectJson_JsonReaded(List<AIAction> actions)
        {
            JsonSerializerSettings options = new JsonSerializerSettings()
            {
                Formatting = Formatting.Indented,
                Converters = { new AIActionsListJsonConverter() }
            };

            string data = JsonConvert.SerializeObject(actions, options);
            var deserialized = JsonConvert.DeserializeObject<List<AIAction>>(data, options);

            Assert.IsTrue(IsActionsSame(actions, deserialized));
        }

        private static IEnumerable<TestCaseData> ActionsGenerator()
        {
            yield return new TestCaseData(new List<AIAction>());
            yield return new TestCaseData(new List<AIAction> { new FleeAction() });
            yield return new TestCaseData(new List<AIAction> { new FleeAction(), new FindAimAction() });
        }

        private static bool IsActionsSame(List<AIAction> lhs, List<AIAction> rhs)
        {
            if (lhs.Count != rhs.Count)
            {
                return false;
            }

            var leftTypes = lhs.Select(a => a.GetType());
            var rightTypes = rhs.Select(a => a.GetType());
            return !leftTypes.Except(rightTypes).Any();
        }
    }
}

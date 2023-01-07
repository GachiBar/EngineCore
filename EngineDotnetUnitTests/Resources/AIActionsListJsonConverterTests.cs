using GameplayCore.AI;
using GameplayCore.AI.Enemy.Actions;
using GameplayCore.Resources;
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
            var actionsList = new AIActionsList();
            actionsList.Actions.AddRange(actions);

            JsonSerializerSettings options = new JsonSerializerSettings()
            {
                Formatting = Formatting.Indented,
                Converters = { new AIActionsListJsonConverter() }
            };

            string data = JsonConvert.SerializeObject(actionsList, options);
            var deserialized = JsonConvert.DeserializeObject<AIActionsList>(data, options);

            Assert.IsTrue(IsActionsSame(actionsList, deserialized));
        }

        private static IEnumerable<TestCaseData> ActionsGenerator()
        {
            yield return new TestCaseData(new List<AIAction>());
            yield return new TestCaseData(new List<AIAction> { new FleeAction() });
            yield return new TestCaseData(new List<AIAction> { new FleeAction(), new FindAimAction() });
        }

        private static bool IsActionsSame(AIActionsList lhs, AIActionsList rhs)
        {
            if (lhs.Actions.Count != rhs.Actions.Count)
            {
                return false;
            }

            var leftTypes = lhs.Actions.Select(a => a.GetType());
            var rightTypes = rhs.Actions.Select(a => a.GetType());
            return !leftTypes.Except(rightTypes).Any();
        }
    }
}

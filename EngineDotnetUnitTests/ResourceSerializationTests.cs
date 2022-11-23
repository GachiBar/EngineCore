using System;
using GameplayCore.Resources;
using GameplayCore.Serialization;
using Newtonsoft.Json;
using NUnit.Framework;

namespace EngineDotnetUnitTests
{
    public class ResourceSerializationTests
    {
        private Resource _resource;
        
        [SetUp]
        public void Setup()
        {
            _resource = new TextAsset("", Guid.NewGuid());
        }

        [Test]
        public void SerializeTest()
        {
            JsonSerializerSettings options = new JsonSerializerSettings()
            {
                Formatting = Formatting.Indented,
                Converters = {new ResourceJsonConverter()}
            };

            string data = JsonConvert.SerializeObject(_resource, options);
            Console.Write(data);
            
            Assert.Pass();
        }

        [Test]
        public void DeserializeTest()
        {
            Assert.Pass();
        }
    }
}
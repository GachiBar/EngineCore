using System;
using System.IO;
using GameplayCore.Resources;
using GameplayCore.Serialization;
using Newtonsoft.Json;
using NUnit.Framework;

namespace EngineDotnetUnitTests
{
    public class ResourceSerializationTests
    {
        private Resource _resource;
        private string _basePath = Path.GetFullPath("..\\..\\..\\.."); // $(SolutionDir)
        
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
            string path = Path.Combine(_basePath, "Assets\\oh_dis_is_file.txt");
            Assert.Pass();
        }

        [Test]
        public void MetadataRead()
        {
            string path = Path.Combine(_basePath, "Assets\\oh_dis_is_file.txt");
            Resource resource = MetadataReader.Read(path);
            Console.WriteLine(resource);
        }
    }
}
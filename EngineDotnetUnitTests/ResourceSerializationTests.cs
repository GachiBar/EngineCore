using System;
using System.IO;
using System.Reflection;
using GameplayCore;
using GameplayCore.Components;
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
        
        private GameObject _gameObject;
        private Scene _scene;
        
        [SetUp]
        public void Setup()
        {
            _resource = new TextAsset("", Guid.NewGuid());
            _scene = new Scene();
            _gameObject = _scene.CreateGameObject();
            
            var component = _gameObject.AddComponent<TestResourceHolderComponent>();
            
            string path = Path.Combine(_basePath, "Assets\\oh_dis_is_file.txt");
            Resource resource = MetadataReader.Read(path);
            component.SetAsset(resource as TextAsset);
            
            MethodInfo invalidate =
                _scene.GetType().GetMethod("Invalidate", BindingFlags.NonPublic | BindingFlags.Instance);
            invalidate.Invoke(_scene, new object[0]);
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
        public void FindByGuidTest()
        {
            string path = Path.Combine(_basePath, "Assets\\oh_dis_is_file.txt");
            Resource resource = MetadataReader.ReadWithoutSave(path);
            
            SetAssetsBasepath();

            Resource readen = MetadataReader.TryGetByGuid(resource.Guid);
            Assert.That(resource.Guid, Is.EqualTo(readen.Guid));
        }
        
        [Test]
        public void ReadTwice_SameRef()
        {
            string path = Path.Combine(_basePath, "Assets\\oh_dis_is_file.txt");
            Resource one = MetadataReader.Read(path);
            Resource two = MetadataReader.Read(path);
            
            Assert.IsTrue(ReferenceEquals(one, two));
        }

        [Test]
        public void MetadataReadTest()
        {
            string path = Path.Combine(_basePath, "Assets\\oh_dis_is_file.txt");
            Resource resource = MetadataReader.Read(path);
            Console.WriteLine(resource);
        }

        [Test]
        public void SerializeGameObjectWithResourceTest()
        {
            JsonSerializerSettings options = new JsonSerializerSettings()
            {
                Formatting = Formatting.Indented,
                Converters =
                {
                    new GameObjectDefaultJsonConverter(_scene), 
                    new ResourceGuidJsonConverter()
                }
            };

            Console.WriteLine(JsonConvert.SerializeObject(_gameObject.GetComponent<TestResourceHolderComponent>() ,options));
            
            GameObject[] container = {_gameObject};
            string data = JsonConvert.SerializeObject(container, options);
            Assert.That(data, Is.Not.EqualTo("[]"));
        }
        
        [Test]
        public void SerializeSceneWithResourceTest()
        {
            string data = _scene.Serialize();
            Console.WriteLine(data);
            Assert.That(data, Is.Not.EqualTo("[]"));
        }

        [Test]
        public void DeserializeGameObjectWithResourceTest()
        {
            SetAssetsBasepath();
            string data = _scene.Serialize();
            
            Scene newScene = new Scene();
            newScene.Deserialize(data);
            
            Console.WriteLine("Done");
        }

        private void SetAssetsBasepath()
        {
            FieldInfo fieldInfo = typeof(MetadataReader).GetField("BasePath", BindingFlags.Static | BindingFlags.NonPublic);
            fieldInfo.SetValue(null, _basePath + "\\Assets");
        }
    }
}
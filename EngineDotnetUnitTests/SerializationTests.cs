using System;
using System.Collections.Generic;
using System.Reflection;
using GameplayCore;
using GameplayCore.Components;
using GameplayCore.Serialization;
using Newtonsoft.Json;
using NUnit.Framework;

namespace EngineDotnetUnitTests
{

    public class Tests
    {
        Scene scene;
        private List<GameObject> objects;

        [SetUp]
        public void Setup()
        {
            scene = new Scene();

            GameObject first = scene.CreateGameObject();
            GameObject second = scene.CreateGameObject();
            objects = new List<GameObject>()
            {
                first, second
            };

            MeshRenderComponent meshRenderer = first.AddComponent<MeshRenderComponent>();
            TransformComponent transform = second.AddComponent<TransformComponent>();
            meshRenderer.GetType().GetField("_transformComponent", BindingFlags.Instance | BindingFlags.NonPublic).SetValue(meshRenderer, transform);

            MethodInfo invalidate =
                scene.GetType().GetMethod("Invalidate", BindingFlags.NonPublic | BindingFlags.Instance);
            invalidate.Invoke(scene, new object[0]);
        }

        [Test]
        public void SerializeTest()
        {
            JsonSerializerSettings options = new JsonSerializerSettings()
            {
                Formatting = Formatting.Indented,
                Converters = {new GameObjectDefaultJsonConverter()}
            };

            string data = JsonConvert.SerializeObject(objects, options);
            string scene_data = scene.Serialize();
            Console.Write(data);
            
            Assert.That(scene_data, Is.EqualTo(data));
            Assert.IsTrue(scene_data != "[]" && scene_data.Length > 0);
        }
        
        [Test]
        public void DeserializeTest()
        {
            JsonSerializerSettings options = new JsonSerializerSettings()
            {
                Formatting = Formatting.Indented,
                Converters = {new GameObjectDefaultJsonConverter()}
            };

            string data = JsonConvert.SerializeObject(objects, options);
            string scene_data = scene.Serialize();
            Console.WriteLine("Original serialize json: ");
            Console.Write(data);

            Scene new_scene = new Scene();
            
            new_scene.Deserialize(scene_data);
            string new_data = new_scene.Serialize();
            
            Console.WriteLine("New json: ");
            Console.Write(new_data);

            Assert.Pass();
        }
    }
}
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

            first.AddComponent<MeshRenderComponent>();
            second.AddComponent<TransformComponent>();

            MethodInfo invalidate =
                scene.GetType().GetMethod("Invalidate", BindingFlags.NonPublic | BindingFlags.Instance);
            invalidate.Invoke(scene, new object[0]);
        }

        [Test]
        public void Test1()
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
    }
}
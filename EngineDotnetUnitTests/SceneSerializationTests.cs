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

    public class SceneTests
    {
        Scene scene;
        private List<GameObject> objects;
        private string special_data;

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

            special_data = @"
[
  {
    ""Guid"": ""93de9056-9ebb-4a6f-85ff-cd9b08f913e1"",
    ""Components"": [
      {
        ""Type"": ""GameplayCore.Components.MeshRenderComponent"",
        ""_gameObject"": ""93de9056-9ebb-4a6f-85ff-cd9b08f913e1""
      },
      {
        ""Type"": ""GameplayCore.Components.TransformComponent"",
        ""_parent"": null,
        ""_localRotation"": {
          ""X"": 0.0,
          ""Y"": 0.0,
          ""Z"": 0.0,
          ""W"": 1.0
        },
        ""_localScale"": {
          ""X"": 1.0,
          ""Y"": 1.0,
          ""Z"": 1.0
        },
        ""_localPosition"": {
          ""X"": 0.0,
          ""Y"": 0.0,
          ""Z"": 0.0
        },
        ""_editorEuler"": {
          ""X"": 0.0,
          ""Y"": 0.0,
          ""Z"": 0.0
        },
        ""_gameObject"": ""93de9056-9ebb-4a6f-85ff-cd9b08f913e1""
      },
      {
        ""Type"": ""GameplayCore.Components.TestEditorFieldsComponent"",
        ""FloatValue"": 0.0,
        ""DoubleValue"": 0.0,
        ""BoolValue"": false,
        ""ByteValue"": 0,
        ""ShortValue"": 0,
        ""IntValue"": 0,
        ""LongValue"": 0,
        ""UbyteValue"": 0,
        ""UshortValue"": 0,
        ""UintValue"": 0,
        ""UlongValue"": 0,
        ""Vector2Value"": {
          ""X"": 0.0,
          ""Y"": 0.0,
          ""IsNormalized"": false,
          ""IsZero"": true
        },
        ""Vector3Value"": {
          ""X"": 0.0,
          ""Y"": 0.0,
          ""Z"": 0.0
        },
        ""Vector4Value"": {
          ""X"": 0.0,
          ""Y"": 0.0,
          ""Z"": 0.0,
          ""W"": 0.0,
          ""IsNormalized"": false,
          ""IsZero"": true
        },
        ""StringValue"": ""TestString"",
        ""GameObjectValue"": null,
        ""_gameObject"": ""93de9056-9ebb-4a6f-85ff-cd9b08f913e1""
      }
    ]
  },
  {
    ""Guid"": ""68525632-518d-432f-98b8-78b2f2ce8e12"",
    ""Components"": [
      {
        ""Type"": ""GameplayCore.Components.CameraComponent"",
        ""FieldOfView"": 90.0,
        ""Near"": 0.1,
        ""Far"": 100.0,
        ""_gameObject"": ""68525632-518d-432f-98b8-78b2f2ce8e12""
      },
      {
        ""Type"": ""GameplayCore.Components.TransformComponent"",
        ""_parent"": null,
        ""_localRotation"": {
          ""X"": 0.0,
          ""Y"": 0.0,
          ""Z"": 0.0,
          ""W"": 1.0
        },
        ""_localScale"": {
          ""X"": 1.0,
          ""Y"": 1.0,
          ""Z"": 1.0
        },
        ""_localPosition"": {
          ""X"": 0.0,
          ""Y"": 0.0,
          ""Z"": 0.0
        },
        ""_editorEuler"": {
          ""X"": 0.0,
          ""Y"": 0.0,
          ""Z"": 0.0
        },
        ""_gameObject"": ""68525632-518d-432f-98b8-78b2f2ce8e12""
      },
      {
        ""Type"": ""GameplayCore.Components.SpectatorComponent"",
        ""_gameObject"": ""68525632-518d-432f-98b8-78b2f2ce8e12""
      }
    ]
  },
  {
    ""Guid"": ""6ddf8e92-1ff0-433d-ad9b-7ecbca974e4f"",
    ""Components"": []
  },
  {
    ""Guid"": ""a81f291e-e198-4f03-bd9f-f9b2f8d54f3a"",
    ""Components"": [
      {
        ""Type"": ""GameplayCore.Components.TransformComponent"",
        ""_parent"": ""93de9056-9ebb-4a6f-85ff-cd9b08f913e1"",
        ""_localRotation"": {
          ""X"": 0.0,
          ""Y"": 0.0,
          ""Z"": 0.0,
          ""W"": 1.0
        },
        ""_localScale"": {
          ""X"": 1.0,
          ""Y"": 1.0,
          ""Z"": 1.0
        },
        ""_localPosition"": {
          ""X"": -3.10252738,
          ""Y"": 0.0,
          ""Z"": 0.0
        },
        ""_editorEuler"": {
          ""X"": 0.0,
          ""Y"": 0.0,
          ""Z"": 0.0
        },
        ""_gameObject"": ""a81f291e-e198-4f03-bd9f-f9b2f8d54f3a""
      },
      {
        ""Type"": ""GameplayCore.Components.MeshRenderComponent"",
        ""_gameObject"": ""a81f291e-e198-4f03-bd9f-f9b2f8d54f3a""
      }
    ]
  }
]
";
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

            Assert.That(new_data, Is.EqualTo(scene_data));
        }
        
        [Test]
        public void DeserializeTest2()
        {
          JsonSerializerSettings options = new JsonSerializerSettings()
          {
            Formatting = Formatting.Indented,
            Converters = {new GameObjectDefaultJsonConverter()}
          };

          List<GameObject> special_object = new List<GameObject>();
          Scene scene = new Scene();
          scene.Deserialize(special_data);
          special_object = JsonConvert.DeserializeObject<List<GameObject>>(special_data, options);

          Assert.Pass();
        }
    }
}
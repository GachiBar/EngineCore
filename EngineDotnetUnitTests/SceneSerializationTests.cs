using System;
using System.Collections.Generic;
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

            first.Name = "Amogus 1";
            second.Name = "Amogus 2";
            
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

            PassRealData();
        }

        private string _basePath = Path.GetFullPath("..\\..\\..\\.."); // $(SolutionDir)
        private void SetAssetsBasepath()
        {
          FieldInfo fieldInfo = typeof(MetadataReader).GetField("BasePath", BindingFlags.Static | BindingFlags.NonPublic);
          fieldInfo.SetValue(null, _basePath + "\\Assets");
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
          SetAssetsBasepath();
          
          JsonSerializerSettings options = new JsonSerializerSettings()
          {
            Formatting = Formatting.Indented,
            Converters = {new GameObjectDefaultJsonConverter()}
          };

          List<GameObject> special_object = new List<GameObject>();
          Scene scene = new Scene();
          scene.Deserialize(special_data);
          // special_object = JsonConvert.DeserializeObject<List<GameObject>>(special_data, options);

          Assert.Pass();
        }
        
         private void PassRealData()
        {
          special_data = @"
[
  {
    ""Guid"": ""b22b3479-88e8-43c9-b900-509e61a44c27"",
    ""Name"": ""Player"",
    ""Components"": [
      {
        ""Type"": ""GameplayCore.Components.MeshRenderComponent"",
        ""Id"": 3,
        ""_asset"": ""363a237f-3a55-4713-b7c6-62d9a52a2f4c"",
        ""_material"": ""4a239380-9b29-4e11-9db9-630b9cb1e349"",
        ""_gameObject"": ""b22b3479-88e8-43c9-b900-509e61a44c27""
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
        ""_gameObject"": ""b22b3479-88e8-43c9-b900-509e61a44c27""
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
        ""_gameObject"": ""b22b3479-88e8-43c9-b900-509e61a44c27""
      }
    ]
  },
  {
    ""Guid"": ""8ac26d58-0b03-41c4-8a22-41c78f0b3481"",
    ""Name"": ""Camera"",
    ""Components"": [
      {
        ""Type"": ""GameplayCore.Components.CameraComponent"",
        ""FieldOfView"": 90.0,
        ""Near"": 0.1,
        ""Far"": 100.0,
        ""_gameObject"": ""8ac26d58-0b03-41c4-8a22-41c78f0b3481""
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
        ""_gameObject"": ""8ac26d58-0b03-41c4-8a22-41c78f0b3481""
      },
      {
        ""Type"": ""GameplayCore.Components.SpectatorComponent"",
        ""_gameObject"": ""8ac26d58-0b03-41c4-8a22-41c78f0b3481""
      }
    ]
  },
  {
    ""Guid"": ""12b2d8ae-c824-4846-ad31-1b39534ec269"",
    ""Name"": ""Empty"",
    ""Components"": []
  }
]
";
        }
    }
}
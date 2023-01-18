using System.Collections;
using System.Collections.Generic;
using System.Linq;
using GameplayCore.Components;
using GameplayCore.Serialization;
using Newtonsoft.Json;

namespace GameplayCore
{
    public class Scene : IReadOnlyList<GameObject>
    {
        private List<GameObject> _gameObjects;
        private List<GameObject> _createdGameObjects;
        private List<GameObject> _deletedGameObjects;
                
        public bool IsInitialized { get; private set; }

        #region IReadOnlyList

        public int Count => _gameObjects.Count;

        public GameObject this[int index] => _gameObjects[index];

        #endregion IReadOnlyList

        public Scene()
        {
            _gameObjects = new List<GameObject>();
            _createdGameObjects = new List<GameObject>();
            _deletedGameObjects = new List<GameObject>();
        }

        #region IReadOnlyList

        public IEnumerator<GameObject> GetEnumerator()
        {
            return _gameObjects.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
           return GetEnumerator();
        }

        #endregion IReadOnlyList

        public void Initialize()
        {
            foreach (var gameObject in _gameObjects)
            {
                gameObject.Initialize();
            }

            IsInitialized = true;
        }

        public void FixedUpdate()
        {
            foreach (var gameObject in _gameObjects)
            {
                gameObject.FixedUpdate();
            }

            Invalidate();
        }

        public void UpdateAI()
        {
            foreach (var gameObject in _gameObjects)
            {
                gameObject.UpdateAI();
            }

            Invalidate();
        }

        public void Update()
        {
            foreach (var gameObject in _gameObjects)
            {
                gameObject.Update();
            }

            Invalidate();
        }               

        public void Render()
        {
            foreach (var gameObject in _gameObjects)
            {
                gameObject.Render();
            }

            Invalidate();
        }

        public void EditorRender()
        {
            foreach (var gameObject in _gameObjects)
            {
                gameObject.EditorRender();
            }

            Invalidate();
        }

        public void RenderGUI()
        {
            foreach (var gameObject in _gameObjects)
            {
                gameObject.RenderGUI();
            }

            Invalidate();
        }

        public void Terminate()
        {
            foreach (var gameObject in _gameObjects)
            {
                gameObject.Terminate();
            }

            IsInitialized = false;
        }

        public string Serialize()
        {
            JsonSerializerSettings options = new JsonSerializerSettings()
            {
                Formatting = Formatting.Indented,
                Converters = 
                {
                    new GameObjectDefaultJsonConverter(),
                    new ResourceGuidJsonConverter(),
                    new AIArbitratorConverter(),
                }
            };

            string data = JsonConvert.SerializeObject(_gameObjects, options);
            return data;
        }

        public void Deserialize(string data)
        {
            // Remove all components from all game objects to trigger OnDetach method.
            foreach (var gameObject in _gameObjects)
            {
                var componentsTypes = gameObject.Select(c => c.GetType()).ToList();

                foreach (var componentType in componentsTypes)
                {
                    gameObject.RemoveComponent(componentType);
                }

                gameObject.Invalidate();
            }

            GameObjectDefaultJsonConverter gameObjectConverter = new GameObjectDefaultJsonConverter(this);
            
            JsonSerializerSettings options = new JsonSerializerSettings()
            {
                Formatting = Formatting.Indented,
                Converters =
                {
                    gameObjectConverter,
                    new ResourceGuidJsonConverter(),
                    new AIArbitratorConverter()
                }
            };

            // Get objects without references
            _gameObjects = JsonConvert.DeserializeObject<List<GameObject>>(data, options);
            
            // And then set it
            gameObjectConverter.PassGuidReferences(data);            

            foreach (var item in _gameObjects)
            {
                TransformComponent component = item.GetComponent<TransformComponent>();
                if(component == null) continue;
                component.FindChildren();
            }

            Invalidate();
        }

        public GameObject CreateGameObject()
        {            
            var gameObject = new GameObject(this, System.Guid.NewGuid());
            _createdGameObjects.Add(gameObject);

            if (IsInitialized)
            {
                gameObject.Initialize();
            }
            
            return gameObject;
        }

        public void DeleteGameObject(GameObject gameObject)
        {
            _createdGameObjects.Remove(gameObject);
            _deletedGameObjects.Add(gameObject);
        }

        public Scene Copy()
        {
            var sceneCopy = new Scene();

            foreach (var gameObject in _gameObjects)
            {
                var gameObjectCopy = sceneCopy.CreateGameObject();

                foreach (var component in gameObject)
                {
                    var componentCopy = component.Copy();
                    gameObjectCopy.AddComponentInternal(componentCopy);
                }
            }

            return sceneCopy;
        }

        internal void Invalidate()
        {
            foreach (var gameObject in _gameObjects)
            {
                gameObject.Invalidate();
            }

            foreach (var gameObject in _deletedGameObjects)
            {
                _gameObjects.Remove(gameObject);
                gameObject.Terminate();
            }

            _deletedGameObjects.Clear();

            foreach (var gameObject in _createdGameObjects)
            {
                _gameObjects.Add(gameObject);
            }

            _createdGameObjects.Clear();
        }
    }
}

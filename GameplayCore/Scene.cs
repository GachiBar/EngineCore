using System.Collections.Generic;

namespace GameplayCore
{
    public class Scene
    {
        private List<GameObject> _gameObjects;
        private List<GameObject> _createdGameObjects;
        private List<GameObject> _deletedGameObjects;

        public bool IsInitialized { get; private set; }

        public Scene()
        {
            _gameObjects = new List<GameObject>();
            _createdGameObjects = new List<GameObject>();
            _deletedGameObjects = new List<GameObject>();
        }

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

        public void Update()
        {
            foreach (var gameObject in _gameObjects)
            {
                gameObject.Update();
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

        public GameObject CreateGameObject()
        {            
            var go = new GameObject(this);
            _createdGameObjects.Add(go);

            if (IsInitialized)
            {
                go.Initialize();
            }
            
            return go;
        }

        public void DeleteGameObject(GameObject go)
        {
            _createdGameObjects.Remove(go);
            _deletedGameObjects.Add(go);
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

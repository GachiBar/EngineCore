using System.Collections.Generic;

namespace GameplayCore
{
    public class Scene
    {
        private List<GameObject> _gameObjects;

        public bool IsInitialized { get; private set; }

        public Scene()
        {
            _gameObjects = new List<GameObject>();
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
        }

        public void Update()
        {
            foreach (var gameObject in _gameObjects)
            {
                gameObject.Update();
            }
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
            _gameObjects.Add(go);
            return go;
        }

        public void DestroyGameObject(GameObject go)
        {
            _gameObjects.Remove(go);
        }
    }
}

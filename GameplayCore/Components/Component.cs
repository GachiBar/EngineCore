using System;
using GameplayCore.Serialization;
using Newtonsoft.Json;

namespace GameplayCore.Components
{
    public abstract class Component
    {
        [SerializeField, JsonConverter(typeof(GameObjectGuidJsonConverter))]
        private GameObject _gameObject = null;

        public GameObject GameObject 
        { 
            get => _gameObject; 
            internal set
            {
                if (_gameObject != null)
                {
                    OnDetach(_gameObject);
                }

                _gameObject = value;

                if (_gameObject != null)
                {
                    OnAttach(_gameObject);
                }                
            } 
        }

        public virtual void Initialize() { }
        public virtual void FixedUpdate() { }
        public virtual void Update() { }
        public virtual void Render() { }
        public virtual void Terminate() { }


        protected virtual void OnAttach(GameObject gameObject) {}
        protected virtual void OnDetach(GameObject gameObject) {}

        string Serialize()
        {
            return String.Empty;
        }

        void Deserialize(string data)
        {
            
        }
    }
}

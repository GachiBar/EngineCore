using System;
using GameplayCore.Serialization;
using Newtonsoft.Json;

namespace GameplayCore.Components
{
    public abstract class Component
    {
        [SerializeField]
        [HideInInspector]
        [JsonConverter(typeof(GameObjectGuidJsonConverter))]
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

        public Component() { }

        public virtual void Initialize() { }
        public virtual void FixedUpdate() { }
        public virtual void Update() { }
        public virtual void Render() { }
        public virtual void Terminate() { }

        /// <summary>
        /// This method called by editor when some field was changed.
        /// </summary>
        /// <param name="fieldName">Field name.</param>        
        internal virtual void Invalidate(string fieldName) { }

        internal string Serialize()
        {
            return String.Empty;
        }

        internal void Deserialize(string data)
        {

        }

        protected virtual void OnAttach(GameObject gameObject) { }
        protected virtual void OnDetach(GameObject gameObject) { }
    }
}

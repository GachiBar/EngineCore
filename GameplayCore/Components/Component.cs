﻿using System;
using GameplayCore.Serialization;

namespace GameplayCore.Components
{
    public abstract class Component
    {
        [SerializeField]
        [HideInInspector]
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
        public virtual void UpdateAI() { }
        public virtual void Update() { }
        public virtual void Render() { }
        public virtual void EditorRender() { }
        public virtual void RenderGUI() { }
        public virtual void Terminate() { }

        public virtual void OnCollisionEnter(RigidbodyComponent rigidbody) { }
        public virtual void OnCollisionStay(RigidbodyComponent rigidbody) { }
        public virtual void OnCollisionExit(RigidbodyComponent rigidbody) { }

        internal virtual Component Copy()
        {
            var componentCopy = (Component)MemberwiseClone();
            componentCopy._gameObject = null;
            return componentCopy;
        }

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

        protected GameObject CreateGameObject()
        {
            return GameObject.Scene.CreateGameObject();
        }

        protected void DeleteGameObject(GameObject gameObject)
        {
            GameObject.Scene.DeleteGameObject(gameObject);
        }
    }
}

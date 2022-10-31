using System;
using System.Reflection;
using System.Linq;
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

        internal string[] GetEditablePropertiesNames()
        {
            return GetType()
                .GetProperties(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance)
                .Where(p => IsEditableProperty(p))
                .Select(p => p.Name)
                .ToArray();
        }

        protected virtual void OnAttach(GameObject gameObject) { }
        protected virtual void OnDetach(GameObject gameObject) { }

        string Serialize()
        {
            return String.Empty;
        }

        void Deserialize(string data)
        {

        }

        private static bool IsEditableProperty(PropertyInfo propertyInfo)
        {
            return IsReadWriteProperty(propertyInfo, false) && IsVisibleInEditor(propertyInfo) ||
                   IsReadWriteProperty(propertyInfo, true) && IsSerializable(propertyInfo);
        }

        private static bool IsReadWriteProperty(PropertyInfo propertyInfo, bool nonPublic)
        {
            return propertyInfo.GetGetMethod(nonPublic) != null 
                && propertyInfo.GetSetMethod(nonPublic) != null;
        }

        private static bool IsVisibleInEditor(PropertyInfo propertyInfo)
        {
            return propertyInfo.CustomAttributes.All(a => a.AttributeType != typeof(HideInInspectorAttribute));
        }

        private static bool IsSerializable(PropertyInfo propertyInfo)
        {
            return propertyInfo.CustomAttributes.Any(a => a.AttributeType == typeof(SerializeFieldAttribute));
        }
    }
}

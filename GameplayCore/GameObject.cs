using System;
using System.Linq;
using System.Collections.Generic;
using GameplayCore.Components;
using System.Collections;

namespace GameplayCore
{
    [Serializable]
    public class GameObject : IReadOnlyList<Component>
    {
        /// <summary>
        /// Flag that determines whether the <see cref="_updatableComponents"/> is in a valid state.
        /// Changing the set of <see cref="GameObject"/> <see cref="Component"/>s leads to an 
        /// invalid state of the <see cref="_updatableComponents"/> collection.
        /// </summary>
        private bool _isUpdatableComponentsInvalid;

        /// <summary>
        /// Dictionary of all <see cref="Component"/>s, presented in this <see cref="GameObject"/>.
        /// </summary>
        private Dictionary<Type, Component> _componentsMap;

        /// <summary>
        /// List of all <see cref="Component"/>s involved in updating.
        /// </summary>
        private List<Component> _updatableComponents;

        /// <summary>
        /// List of removed <see cref="Component"/>s. We should terminate it.
        /// </summary>
        private List<Component> _removedComponents;

        /// <summary>
        /// <see cref="Scene"/>, which contain this <see cref="GameObject"/>
        /// </summary>
        public readonly Scene Scene;

        /// <summary>
        /// Flag that determines, is this <see cref="GameObject"/> is initialized.
        /// </summary>
        public bool IsInitialized { get; private set; }
        
        /// <summary>
        /// Global unique identifier of <see cref="Component"/>s. Initialized by scene.
        /// </summary>
        private System.Guid _guid;
        
        /// <summary>
        /// Name of <see cref="Component"/>s.
        /// </summary>
        private String Name;

        #region IReadOnlyList

        public int Count => _updatableComponents.Count;

        public Component this[int index] => _updatableComponents[index];

        #endregion IReadOnlyList

        public System.Guid Guid => _guid;
        
        public event Action<GameObject, Component> ComponentAdded;
        public event Action<GameObject, Component> ComponentRemoved;

        internal GameObject(Scene scene, System.Guid guid)
        {
            _componentsMap = new Dictionary<Type, Component>();
            _updatableComponents = new List<Component>();
            _removedComponents = new List<Component>();
            _isUpdatableComponentsInvalid = false;
            Scene = scene;
            _guid = guid;
        }

        public void Initialize()
        {
            foreach (var component in _updatableComponents)
            {
                component.Initialize();
            }

            IsInitialized = true;
        }

        public void FixedUpdate()
        {
            foreach (var component in _updatableComponents)
            {
                component.FixedUpdate();
            }
        }

        public void Update()
        {
            foreach (var component in _updatableComponents)
            {
                component.Update();
            }
        }

        public void Render()
        {
            foreach (var component in _updatableComponents)
            {
                component.Render();
            }
        }

        public void Terminate()
        {
            foreach (var component in _updatableComponents)
            {
                component.Terminate();
            }

            IsInitialized = false;
        }

        public T AddComponent<T>() where T : Component, new()
        {
            return (T)AddComponent(typeof(T));
        }

        public Component AddComponent(Type componentType)
        {
            if (!(Activator.CreateInstance(componentType) is Component instance))
            {
                throw new ArgumentException(
                    $"Not valid type. It should inherit from {typeof(Component)} and have default constructor.");
            }

            if (_componentsMap.TryGetValue(componentType, out var component))
            {
                RemoveComponent(componentType);
            }

            _componentsMap[instance.GetType()] = instance;
            instance.GameObject = this;

            if (IsInitialized)
            {
                instance.Initialize();
            }

            ComponentAdded?.Invoke(this, instance);
            _isUpdatableComponentsInvalid = true;
            return instance;
        }

        // For serialization
        private void AddComponentSilent(Component instance)
        {
            if (_componentsMap.TryGetValue(instance.GetType(), out _))
            {
                RemoveComponent(instance.GetType());
            }

            _componentsMap[instance.GetType()] = instance;
            instance.GameObject = this;
        }

        public T RemoveComponent<T>() where T : Component
        {
            return (T)RemoveComponent(typeof(T));
        }

        public Component RemoveComponent(Type componentType)
        {
            if (_componentsMap.TryGetValue(componentType, out var component))
            {
                _componentsMap.Remove(componentType);

                if (IsInitialized)
                {
                    _removedComponents.Add(component);
                }
                                
                component.GameObject = null;                           
                _isUpdatableComponentsInvalid = true;
                return component;
            }

            return null;
        }
        
        public T GetComponent<T>() where T : Component
        {
            return (T)GetComponent(typeof(T));
        }

        public Component GetComponent(Type componentType)
        {
            if (_componentsMap.TryGetValue(componentType, out var component))
            {
                return component;
            }

            return null;
        }
        
        public IEnumerable<Component> GetComponentsEnumerable()
        {
            foreach (var component in _componentsMap.Values)
            {
                yield return component;
            }
        }

        internal void Invalidate()
        {
            if (_isUpdatableComponentsInvalid)
            {
                foreach (var component in _removedComponents)
                {                    
                    component.Terminate();
                    ComponentRemoved?.Invoke(this, component);
                }

                _updatableComponents = _componentsMap.Values.ToList();
                _isUpdatableComponentsInvalid = false;
            }
        }

        #region IReadOnlyList

        public IEnumerator<Component> GetEnumerator()
        {
            return _updatableComponents.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        #endregion IReadOnlyList
    }
}

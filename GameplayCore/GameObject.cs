using System;
using System.Linq;
using System.Collections.Generic;

namespace GameplayCore
{
    public class GameObject
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

        internal GameObject(Scene scene)
        {
            _componentsMap = new Dictionary<Type, Component>();
            _updatableComponents = new List<Component>();
            _removedComponents = new List<Component>();
            _isUpdatableComponentsInvalid = false;
            Scene = scene;           
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

            _isUpdatableComponentsInvalid = true;
            return instance;
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
            return _componentsMap.TryGetValue(componentType, out var component) 
                ? component 
                : null;
        }

        internal void Invalidate()
        {
            if (_isUpdatableComponentsInvalid)
            {
                foreach (var component in _removedComponents)
                {
                    component.Terminate();
                }

                _updatableComponents = _componentsMap.Values.ToList();
                _isUpdatableComponentsInvalid = false;
            }
        }
    }
}

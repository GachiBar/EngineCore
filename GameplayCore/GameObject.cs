using System;
using System.Collections.Generic;

namespace GameplayCore
{
    public class GameObject
    {
        //TODO: Handle component addition/deletation per frame. They can break iterators.
        private Dictionary<Type, Component> _componentsMap;

        public readonly Scene Scene;

        public bool IsInitialized { get; private set; }

        internal GameObject(Scene scene)
        {
            _componentsMap = new Dictionary<Type, Component>();
            Scene = scene;           
        }

        public void Initialize()
        {
            foreach (var component in _componentsMap.Values)
            {
                component.Initialize();
            }

            IsInitialized = true;
        }

        public void FixedUpdate()
        {
            foreach (var component in _componentsMap.Values)
            {
                component.FixedUpdate();
            }
        }

        public void Update()
        {
            foreach (var component in _componentsMap.Values)
            {
                component.Update();
            }
        }

        public void Terminate()
        {
            foreach (var component in _componentsMap.Values)
            {
                component.Terminate();
            }

            IsInitialized = false;
        }

        public T AddComponent<T>() where T : Component, new()
        {
            var component = new T();
            component.GameObject = this;

            if (IsInitialized)
            {
                component.Initialize();
            }
           
            return component;
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
                RemoveComponent(component.GetType());
            }

            _componentsMap[instance.GetType()] = instance;
            instance.GameObject = this;

            if (IsInitialized)
            {
                instance.Initialize();
            }

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
                    component.Terminate();
                }

                component.GameObject = null;
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

        }
    }
}

using System;
using System.Collections.Generic;

namespace GameplayCore
{
    public class GameObject
    {
        private Dictionary<Type, Component> _componentsMap;

        public readonly Scene Scene;

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
        }

        public T AddComponent<T>() where T : Component, new()
        {
            var component = new T();
            component.GameObject = this;

            if (Scene.IsInitialized)
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
                Remove(component.GetType());
            }

            _componentsMap[instance.GetType()] = instance;
            instance.GameObject = this;

            if (Scene.IsInitialized)
            {
                instance.Initialize();
            }

            return instance;
        }

        public T Remove<T>() where T : Component
        {
            return (T) Remove(typeof(T));
        }

        public Component Remove(Type componentType)
        {
            if (_componentsMap.TryGetValue(componentType, out var component))
            {
                _componentsMap.Remove(componentType);

                if (Scene.IsInitialized)
                {
                    component.Terminate();
                }

                component.GameObject = null;
                return component;
            }

            return null;
        }  
    }
}

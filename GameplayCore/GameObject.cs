using System;
using System.Linq;
using System.Collections.Generic;
using GameplayCore.Components;
using System.Collections;
using GameplayCore.Editor;
using GameplayCore.Serialization;

namespace GameplayCore
{
    [Serializable]
    public class GameObject : IReadOnlyList<Component>
    {        
        private const int MaxNameSize = 255;

        /// <summary>
        /// Global unique identifier of <see cref="Component"/>s. Initialized by scene.
        /// </summary>
        private Guid _guid;

        /// <summary>
        /// Name of <see cref="GameObject"/>.
        /// </summary>
        [InspectorName("Name")]
        [SerializeField]
        private string _name;

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

        public Guid Guid => _guid;

        /// <summary>
        /// Name of <see cref="GameObject"/>.
        /// </summary>
        public string Name
        { 
            get => _name;
            set
            {
                if (value == null)
                {
                    throw new ArgumentNullException($"{nameof(Name)} can not be null.");
                }

                var name = value.Trim();

                if (name.Length > MaxNameSize || string.IsNullOrEmpty(name))
                {
                    throw new ArgumentOutOfRangeException($"{nameof(Name)} length should be in range (0, {MaxNameSize}].");
                }

                _name = name;
            }
        }

        #region IReadOnlyList

        public int Count
        {
            get
            {
                return _updatableComponents.Count;
            }
        }

        public Component this[int index] => _updatableComponents[index];

        #endregion IReadOnlyList       
        
        public event Action<GameObject, Component> ComponentAdded;
        public event Action<GameObject, Component> ComponentRemoved;

        internal GameObject(Scene scene, Guid guid)
        {
            _guid = guid;
            _componentsMap = new Dictionary<Type, Component>();
            _updatableComponents = new List<Component>();
            _removedComponents = new List<Component>();
            _isUpdatableComponentsInvalid = false;
            Scene = scene;
            Name = $"GameObject";
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

        public void UpdateAI()
        {
            foreach (var component in _updatableComponents)
            {
                component.UpdateAI();
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

        public void EditorRender()
        {
            foreach (var component in _updatableComponents)
            {
                component.EditorRender();
            }
        }

        public void RenderGUI()
        {
            foreach (var component in _updatableComponents)
            {
                component.RenderGUI();
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

        public void CollisionEnter(RigidbodyComponent rigidbody)
        {
            foreach(var component in _updatableComponents)
            {
                component.OnCollisionEnter(rigidbody);
            }
        }

        public void CollisionStay(RigidbodyComponent rigidbody)
        {
            foreach (var component in _updatableComponents)
            {
                component.OnCollisionStay(rigidbody);
            }
        }

        public void CollisionExit(RigidbodyComponent rigidbody)
        {
            foreach (var component in _updatableComponents)
            {
                component.OnCollisionExit(rigidbody);
            }
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

            AddComponentInternal(instance);
            return instance;
        }

        internal void AddComponentInternal(Component instance)
        {
            if (_componentsMap.TryGetValue(instance.GetType(), out _))
            {
                RemoveComponent(instance.GetType());
            }

            _componentsMap[instance.GetType()] = instance;
            instance.GameObject = this;

            if (IsInitialized)
            {
                instance.Initialize();
            }

            ComponentAdded?.Invoke(this, instance);
            _isUpdatableComponentsInvalid = true;
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
                _removedComponents.Add(component);                                       
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

        public GameObject Copy()
        {
            var gameObjectCopy = Scene.CreateGameObject();
            gameObjectCopy._name = _name;
            
            foreach (var component in _componentsMap.Values)
            {
                var componentCopy = component.Copy();
                gameObjectCopy.AddComponentInternal(componentCopy);
            }

            gameObjectCopy.Invalidate();
            return gameObjectCopy;
        }

        internal void Invalidate()
        {          
            if (_isUpdatableComponentsInvalid)
            {
                foreach (var component in _removedComponents)
                {
                    if (IsInitialized)
                    {
                        component.Terminate();
                    }
                                       
                    component.GameObject = null;
                    ComponentRemoved?.Invoke(this, component);
                }

                _removedComponents.Clear();
                _updatableComponents = _componentsMap.Values.ToList();
                _isUpdatableComponentsInvalid = false;
            }
        }

        #region IReadOnlyList

        public IEnumerator<Component> GetEnumerator()
        {
            return _componentsMap.Values.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        #endregion IReadOnlyList
    }
}

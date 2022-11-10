using System;

namespace GameplayCore.Components
{
    public class TestHierarchyComponent : Component
    {
        private int _childrenCount;
        private TransformComponent _transformComponent = null;

        public int ChildrentCount 
        {
            get => _childrenCount;
            set
            {
                _childrenCount = Math.Max(value, 0);
                UpdateChildren();
            }
        }

        protected override void OnAttach(GameObject gameObject)
        {
            _transformComponent = GameObject.GetComponent<TransformComponent>();

            gameObject.ComponentAdded += OnComponentAdded;
            gameObject.ComponentRemoved += OnComponentRemoved;
        }

        protected override void OnDetach(GameObject gameObject)
        {
            gameObject.ComponentAdded -= OnComponentAdded;
            gameObject.ComponentRemoved -= OnComponentRemoved;
        }

        private void OnComponentAdded(GameObject gameObject, Component component)
        {
            if (component is TransformComponent transformComponent)
            {
                _transformComponent = transformComponent;
            }
        }

        private void OnComponentRemoved(GameObject gameObject, Component component)
        {
            if (component is TransformComponent)
            {
                _transformComponent = null;
            }
        }

        private void UpdateChildren()
        {
            while (_transformComponent.ChildrenCount > _childrenCount)
            {
                var child = _transformComponent[0];
                child.Parent = null;
                GameObject.Scene.DeleteGameObject(child.GameObject);
            }

            while (_transformComponent.ChildrenCount < _childrenCount)
            {
                var child = GameObject.Scene.CreateGameObject();
                var transform = child.AddComponent<TransformComponent>();
                transform.Parent = _transformComponent;
            }
        }
    }
}

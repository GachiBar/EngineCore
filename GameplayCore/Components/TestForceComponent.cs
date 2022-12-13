using GameplayCore.Mathematics;

namespace GameplayCore.Components
{
    public class TestForceComponent : Component
    {
        private RigidbodyComponent _rigidbodyComponent;

        public Vector3 Force;

        public override void FixedUpdate()
        {
            if (_rigidbodyComponent != null)
            {
                _rigidbodyComponent.AddForce(Force);
            }
        }

        protected override void OnAttach(GameObject gameObject)
        {
            _rigidbodyComponent = GameObject.GetComponent<RigidbodyComponent>();

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
            if (component is RigidbodyComponent rigidbodyComponent)
            {
                _rigidbodyComponent = rigidbodyComponent;
            }
        }

        private void OnComponentRemoved(GameObject gameObject, Component component)
        {
            if (component is RigidbodyComponent)
            {
                _rigidbodyComponent = null;
            }
        }
    }
}

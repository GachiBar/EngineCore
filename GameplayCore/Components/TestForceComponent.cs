using GameplayCore.Mathematics;
using System;

namespace GameplayCore.Components
{
    public class TestForceComponent : Component
    {
        private BoxBodyComponent _bodyComponent;

        public Vector3 Force;

        public override void FixedUpdate()
        {
            if (_bodyComponent != null)
            {
                _bodyComponent.AddForce(Force);
            }
        }

        protected override void OnAttach(GameObject gameObject)
        {
            _bodyComponent = GameObject.GetComponent<BoxBodyComponent>();

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
            if (component is BoxBodyComponent bodyComponent)
            {
                _bodyComponent = bodyComponent;
            }
        }

        private void OnComponentRemoved(GameObject gameObject, Component component)
        {
            if (component is BoxBodyComponent)
            {
                _bodyComponent = null;
            }
        }
    }
}

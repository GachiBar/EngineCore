using GameplayCore.Serialization;

namespace GameplayCore.Components
{
    public class MeshRenderComponent : Component
    {
        private static ulong _id = 0;
        [SerializeField] private TransformComponent _transformComponent = null;

        public readonly ulong Id;
        public string Path;

        public MeshRenderComponent()
        {
            Id = _id;
            _id += 1;
        }

        public override void Initialize() 
        {
            EngineApi.Render.RegisterModel(Id);
        }

        public override void Render()
        {
            if (_transformComponent != null)
            {
                EngineApi.Render.DrawModel(Id, _transformComponent.ModelMatrix);
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
    }
}

namespace GameplayCore.Components
{
    public class MeshRenderComponent : Component
    {
        private static ulong _id = 0;
        private TransformComponent _transformComponent = null;
        //Material{Texture, Metalic, Raffness, Normal}
        public ulong Id { get; private set; }

        public MeshRenderComponent()
        {
            Id = _id;
            _id += 1;
        }

        public override void Initialize() 
        {
            EngineApi.RenderApi.RegisterModel(Id);
        }

        public override void Render()
        {
            if (_transformComponent != null)
            {
                EngineApi.RenderApi.DrawModel(Id, _transformComponent.ModelMatrix);
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

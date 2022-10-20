namespace GameplayCore.Components
{
    public class MeshRenderComponent : Component
    {
        private static ulong _id = 0;
        private TransformComponent _transform;

        public readonly ulong Id;

        public string Path;

        public MeshRenderComponent()
        {
            Id = _id;
            _id += 1;
        }

        public override void Initialize() 
        {
            _transform = GameObject.GetComponent<TransformComponent>();
            _transform.Position += new Mathematics.Vector3(0.0f, 0.0f, 5.0f);
            EngineApi.Render.RegisterModel(Id);
        }

        public override void Render()
        {
            EngineApi.Render.DrawModel(Id, _transform.ModelMatrix);
        }

        public override void Terminate() 
        { 
        
        }
    }
}

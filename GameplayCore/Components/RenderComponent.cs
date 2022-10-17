using GameplayCore.Mathematics;

namespace GameplayCore.Components
{
    public class RenderComponent : Component
    {
        private static ulong _id = 0;
        private TransformComponent _transform;

        public ulong Id;

        public RenderComponent()
        {
            Id = _id;
            _id += 1;
        }

        public override void Initialize() 
        {
            _transform = GameObject.GetComponent<TransformComponent>();
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

namespace GameplayCore
{
    public class RenderComponent : Component
    {
        private static ulong _id = 0;

        public ulong Id;

        public RenderComponent()
        {
            Id = _id;
            _id += 1;
        }

        public override void Initialize() 
        {
            Renderer.RegisterModel(Id);
        }

        public override void Render()
        {
            Renderer.DrawModel(Id);
        }

        public override void Terminate() 
        { 
        
        }
    }
}

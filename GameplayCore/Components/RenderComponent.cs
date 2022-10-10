using GameplayCore.Mathematics;

namespace GameplayCore.Components
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
            EngineApi.Render.RegisterModel(Id);
        }

        public override void Render()
        {
            var lhs = new Vector3(1, 2, 3);
            var rhs = new Vector3(3, 2, 1);
            var result = lhs + rhs;
            System.Console.WriteLine($"(X: {result.X}; Y: {result.Y}; Z: {result.Z})");
            
            var magnitude = result.Magnitude;
            System.Console.WriteLine($"Magnitude: {magnitude}");

            var sqrMagnitude = result.MagnitudeSquared;
            System.Console.WriteLine($"Magnitude squared: {sqrMagnitude}");

            var normolized = result.Normolized;
            System.Console.WriteLine($"(X: {normolized.X}; Y: {normolized.Y}; Z: {normolized.Z})");

            EngineApi.Render.DrawModel(Id);
        }

        public override void Terminate() 
        { 
        
        }
    }
}

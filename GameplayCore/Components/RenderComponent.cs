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
            EngineApi.Render.Internal_RegisterModel(Id);
        }

        public override void Render()
        {
            Quaternion q = Quaternion.Identity;
            System.Console.WriteLine(q);
            q.SetFromToRotation(Vector3.Up, Vector3.Right);
            System.Console.WriteLine(q);
            var qq = Quaternion.RotateTowards(Quaternion.Identity, q, 10);
            System.Console.WriteLine(qq);
            var qr = Quaternion.LookRotation(Vector3.One);
            System.Console.WriteLine(qr);
            System.Console.WriteLine(q.EulerAngles);
            q.EulerAngles = new Vector3(30, 0, 0);
            System.Console.WriteLine(q.EulerAngles);

            EngineApi.Render.Internal_DrawModel(Id);
        }

        public override void Terminate() 
        { 
        
        }
    }
}

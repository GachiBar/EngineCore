using System;

namespace GameplayCore
{
    public class TestFixedUpdateComponent : Component
    {
        public TestFixedUpdateComponent()
        {

        }

        public override void Initialize()
        {
            Console.WriteLine($"{nameof(TestFixedUpdateComponent)} Initialized.");
        }

        public override void FixedUpdate()
        {
            Console.WriteLine($"{nameof(TestFixedUpdateComponent)} FixedUpdate.");
        }

        public override void Terminate()
        {
            Console.WriteLine($"{nameof(TestFixedUpdateComponent)} Terminated.");
        }
    }
}

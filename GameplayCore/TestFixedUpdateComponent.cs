using System;

namespace GameplayCore
{
    public class TestFixedUpdateComponent : Component
    {
        private static int id_ = 0;

        public TestFixedUpdateComponent()
        {
            id_ += 1;
        }

        public override void Initialize()
        {
            Console.WriteLine($"{nameof(TestFixedUpdateComponent)}({id_}) Initialized.");
        }

        public override void FixedUpdate()
        {
            Console.WriteLine($"{nameof(TestFixedUpdateComponent)}({id_}) FixedUpdate.");
        }

        public override void Terminate()
        {
            Console.WriteLine($"{nameof(TestFixedUpdateComponent)}({id_}) Terminated.");
        }
    }
}

using System;

namespace GameplayCore.Components
{
    public class TestUpdateComponent : Component
    {
        private static int id_ = 0;

        public TestUpdateComponent()
        {
            id_ += 1;
        }

        public override void Initialize()
        {
            Console.WriteLine($"{nameof(TestUpdateComponent)}({id_}) Initialized.");
        }

        public override void Update()
        {
            Console.WriteLine($"{nameof(TestUpdateComponent)}({id_}) Updated.");
        }

        public override void Terminate()
        {
            Console.WriteLine($"{nameof(TestUpdateComponent)}({id_}) Terminated.");
        }
    }
}

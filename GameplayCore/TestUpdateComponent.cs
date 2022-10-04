using System;

namespace GameplayCore
{
    public class TestUpdateComponent : Component
    {
        public TestUpdateComponent()
        { 
        
        }

        public override void Initialize()
        {
            Console.WriteLine($"{nameof(TestUpdateComponent)} Initialized.");
        }

        public override void Update()
        {
            Console.WriteLine($"{nameof(TestUpdateComponent)} Updated.");
        }

        public override void Terminate()
        {
            Console.WriteLine($"{nameof(TestUpdateComponent)} Terminated.");
        }
    }
}

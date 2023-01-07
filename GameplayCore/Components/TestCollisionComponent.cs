using System;

namespace GameplayCore.Components
{
    public class TestCollisionComponent : Component
    {
        public override void OnCollisionEnter(RigidbodyComponent rigidbody) 
        {
            Console.WriteLine($"Collision enter: {rigidbody.GameObject.Name}");
        }

        public override void OnCollisionStay(RigidbodyComponent rigidbody) 
        {
            Console.WriteLine($"Collision stay: {rigidbody.GameObject.Name}");
        }

        public override void OnCollisionExit(RigidbodyComponent rigidbody) 
        {
            Console.WriteLine($"Collision exit: {rigidbody.GameObject.Name}");
        }
    }
}

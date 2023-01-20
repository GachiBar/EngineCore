using System;

namespace GameplayCore.Components
{
    public class RemoveOnTouchComponent : Component
    {
        public override void OnCollisionEnter(RigidbodyComponent rigidbody)
        {
            if (rigidbody.GameObject.GetComponent<FpsControllerComponent>() == null)
            {
                return;
            }

            Log.PrintMessage("Pickuped!", Duration: 0.0f);
            Console.WriteLine("PUp");

            DeleteGameObject(GameObject);
        }
    }
}

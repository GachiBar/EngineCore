using System;

namespace GameplayCore
{
    public class GameObject
    {
        public void Update(float dt)
        {
            Console.WriteLine($"Delta time: {dt}");
        }
    }
}

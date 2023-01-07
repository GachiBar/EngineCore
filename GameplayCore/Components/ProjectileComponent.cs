namespace GameplayCore.Components
{
    public class ProjectileComponent : Component
    {
        public float Damage = 30.0f;

        public override void OnCollisionEnter(RigidbodyComponent rigidbody) 
        {
            var other = rigidbody.GameObject;
            var health = other.GetComponent<HealthComponent>();

            if (health != null)
            {
                health.Damage(Damage);
                DeleteGameObject(GameObject);
            }
        }
    }
}

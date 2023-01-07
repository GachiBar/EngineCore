using GameplayCore.AI.Enemy.Goals;
using GameplayCore.Components;

namespace GameplayCore.AI.Enemy
{
    public class EnemyArbitrator : AIArbitrator
    {
        public override AIGoal ChooseGoal(GameObject gameObject, AIState state)
        {
            UpdateStete(gameObject, state);

            if (state.GetBoolValue("NeedFlee"))
            {
                return new FleeGoal();
            }
            if (state.GetBoolValue("NeedReloading") == true)
            {
                return new ReloadingGoal();                
            }

            return new ShootGoal();
        }

        private void UpdateStete(GameObject gameObject, AIState state)
        {
            var health = gameObject.GetComponent<HealthComponent>();
            var needFlee = (health.Health / health.MaxHealth) < 0.5f;
            state.SetBoolValue("NeedFlee", needFlee);
        }
    }
}

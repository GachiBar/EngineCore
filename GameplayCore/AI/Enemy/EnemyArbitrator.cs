using GameplayCore.AI.Enemy.Goals;
using GameplayCore.Components;
using System;

namespace GameplayCore.AI.Enemy
{
    public class EnemyArbitrator : AIArbitrator
    {
        public override void InitState(AIState state)
        {
            state.SetGameObjectValue("Aim", null);
            state.SetBoolValue("HasAim", false);
            state.SetBoolValue("CanShoot", false);
            state.SetBoolValue("NeedReloading", false);
            state.SetBoolValue("NeedFlee", false);
            state.SetFloatValue("ReloadingTime", 0.75f);
        }

        public override AIGoal ChooseGoal(GameObject gameObject, AIState state)
        {
            UpdateState(gameObject, state);

            if (state.GetBoolValue("NeedFlee") == true)
            {
                return new FleeGoal();
            }
            if (state.GetBoolValue("NeedReloading") == true)
            {
                return new ReloadingGoal();                
            }

            return new ShootGoal();
        }

        private void UpdateState(GameObject gameObject, AIState state)
        {
            var health = gameObject.GetComponent<HealthComponent>();
            var needFlee = (health.Health / health.MaxHealth) < 0.5f;
            state.SetBoolValue("NeedFlee", needFlee);
        }
    }
}

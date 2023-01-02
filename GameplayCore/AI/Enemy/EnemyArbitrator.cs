using GameplayCore.AI.Enemy.Goals;

namespace GameplayCore.AI.Enemy
{
    public class EnemyArbitrator : Arbitrator
    {
        public override Goal ChooseGoal(GameObject gameObject, State state)
        {
            if (state.GetBoolValue("NeedReloading") == true)
            {
                return new ReloadGoal();                
            }

            return new ShootGoal();
        }
    }
}

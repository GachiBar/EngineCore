namespace GameplayCore.AI.Enemy.Goals
{
    public class ShootGoal : AIGoal
    {
        public override int DistanceTo(AIState state)
        {
            if (state.GetBoolValue("NeedReloading") == true)
            {
                return 0;
            }
            if (state.GetBoolValue("CanShoot") == true)
            {
                return 1;
            }
            if (state.GetBoolValue("HasAim") == true)
            {
                return 2;
            }

            return 3;
        }
    }
}

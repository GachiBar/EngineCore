namespace GameplayCore.AI.Enemy.Goals
{
    public class ReloadingGoal : AIGoal
    {
        public override int DistanceTo(AIState state)
        {
            if (state.GetBoolValue("NeedReloading") == false)
            {
                return 0;
            }

            return 1;
        }
    }
}

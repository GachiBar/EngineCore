namespace GameplayCore.AI.Enemy.Goals
{
    public class ReloadGoal : Goal
    {
        public override int DistanceTo(State state)
        {
            if (state.GetBoolValue("NeedReloading") == false)
            {
                return 0;
            }

            return 1;
        }
    }
}

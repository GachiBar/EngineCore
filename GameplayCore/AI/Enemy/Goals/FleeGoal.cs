namespace GameplayCore.AI.Enemy.Goals
{
    public class FleeGoal : Goal
    {
        public override int DistanceTo(State state)
        {
            if (state.GetBoolValue("NeedFlee") == false)
            {
                return 0;
            }

            return 1;
        }
    }
}

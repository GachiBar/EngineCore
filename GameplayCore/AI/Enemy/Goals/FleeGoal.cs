namespace GameplayCore.AI.Enemy.Goals
{
    public class FleeGoal : AIGoal
    {
        public override int DistanceTo(AIState state)
        {
            if (state.GetBoolValue("NeedFlee") == false)
            {
                return 0;
            }

            return 1;
        }
    }
}

namespace GameplayCore.AI
{
    public enum GoalPriority
    {
        High,
        Average,
        Low,
    }

    public abstract class Goal
    {
        public abstract int DistanceTo(State state);
        public abstract GoalPriority GetPriority();
    }
}

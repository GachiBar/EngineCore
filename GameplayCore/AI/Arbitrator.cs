namespace GameplayCore.AI
{
    public abstract class Arbitrator
    {
        public abstract void ChooseGoal(GameObject gameObject, State state);
    }
}

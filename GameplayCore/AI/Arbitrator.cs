namespace GameplayCore.AI
{
    public abstract class Arbitrator
    {
        public abstract Goal ChooseGoal(GameObject gameObject, State state);
    }
}

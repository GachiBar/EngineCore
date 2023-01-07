namespace GameplayCore.AI
{
    public abstract class AIArbitrator
    {
        public abstract AIGoal ChooseGoal(GameObject gameObject, AIState state);
    }
}

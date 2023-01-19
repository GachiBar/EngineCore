namespace GameplayCore.AI
{
    public abstract class AIArbitrator
    {
        public abstract void InitState(AIState state);
        public abstract AIGoal ChooseGoal(GameObject gameObject, AIState state);
    }
}

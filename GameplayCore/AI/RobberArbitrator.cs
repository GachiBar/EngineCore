using GameplayCore.AI.CaravanAI.Goals;
using GameplayCore.AI.Enemy.Goals;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameplayCore.AI
{
    public class RobberArbitrator : AIArbitrator
    {
        public override AIGoal ChooseGoal(GameObject gameObject, AIState state)
        {
            if (state.GetBoolValue("isRetreating") == true)
            {
                return new RetreatGoal();
            }
            if(state.GetBoolValue("collectedGold") ==  true)
                return new HideGoldGoal();
            return new CollectGoldGoal();
        }

        public override void InitState(AIState state)
        {
            state.SetBoolValue("isRetreating", false);
            state.SetBoolValue("reachedBase", false);
            state.SetBoolValue("isNearCaravan", false);
            state.SetBoolValue("collectedGold", false);
        }
    }
}

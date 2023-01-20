using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameplayCore.AI.CaravanAI.Goals
{
    public class CollectGoldGoal : AIGoal
    {
        public override int DistanceTo(AIState state)
        {
            if (state.GetBoolValue("collectedGold") && state.GetBoolValue("isNearCaravan"))
            {
                return 0;
            }
            if(state.GetBoolValue("isNearCaravan") && !state.GetBoolValue("collectedGold"))
                return 1;
            return 2;
        }
    }
}

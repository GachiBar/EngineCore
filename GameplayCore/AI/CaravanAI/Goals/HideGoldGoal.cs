using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameplayCore.AI.CaravanAI.Goals
{
    internal class HideGoldGoal : AIGoal
    {
        public override int DistanceTo(AIState state)
        {
            if(state.GetBoolValue("reachedBase") && !state.GetBoolValue("collectedGold")) return 0;
            return 1;
        }
    }
}

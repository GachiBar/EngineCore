using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameplayCore.AI.CaravanAI.Goals
{
    public class GuardRoamGoal : AIGoal
    {
        public override int DistanceTo(AIState state)
        {
            if (!state.GetBoolValue("isIdle"))
            {
                if (state.GetBoolValue("hasTarget"))
                    return 2;
            }
            else if (!state.GetBoolValue("isNearBaseTile"))
                return 1;
            return 0;
        }
    }
}

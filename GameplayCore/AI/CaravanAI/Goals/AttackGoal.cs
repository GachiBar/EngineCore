using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameplayCore.AI.CaravanAI.Goals
{
    public class AttackGoal : AIGoal
    {
        public override int DistanceTo(AIState state)
        {
            if (!state.GetBoolValue("isNearTarget") && state.GetBoolValue("hasTarget"))
                return 2;
            else {
                if (state.GetBoolValue("isNearTarget") && state.GetBoolValue("hasTarget"))
                    return 1;
                else return 0;
            }
        }
    }
}

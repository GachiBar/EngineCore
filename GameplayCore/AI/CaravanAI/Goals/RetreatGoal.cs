using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameplayCore.AI.CaravanAI.Goals
{
    public class RetreatGoal : AIGoal
    {
        public override int DistanceTo(AIState state)
        {
            if (state.GetBoolValue("reachedBase") && !state.GetBoolValue("isRetreating"))
            {
                return 0;
            }

            if (state.GetBoolValue("isRetreating") && !state.GetBoolValue("reachedBase"))
            {
                return 1;
            }

            return 2;
            //throw new NotImplementedException();
        }
    }
}

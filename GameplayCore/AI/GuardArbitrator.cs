using GameplayCore.AI.CaravanAI.Goals;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameplayCore.AI
{
    public class GuardArbitrator : AIArbitrator
    {
        public override AIGoal ChooseGoal(GameObject gameObject, AIState state)
        {
            if (state.GetBoolValue("hasTarget"))
                return new GuardAttackGoal();
            return new GuardRoamGoal();
        }

        public override void InitState(AIState state)
        {
            state.SetBoolValue("isIdle", true);
            state.SetBoolValue("hasTarget", false);
            state.SetBoolValue("isNearTarget", false);
            state.SetBoolValue("isNearBaseTile", false);
        }
    }
}

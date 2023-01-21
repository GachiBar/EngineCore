using GameplayCore.AI.CaravanAI.Goals;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameplayCore.AI
{
    public class WarriorArbitrator : AIArbitrator
    {
        public override AIGoal ChooseGoal(GameObject gameObject, AIState state)
        {
            if (state.GetBoolValue("isRetreating"))
            {
                return new RetreatGoal();
            }
            if (state.GetBoolValue("hasTarget"))
                return new AttackGoal();
            return new IdleGoal();
        }

        public override void InitState(AIState state)
        {
            state.SetBoolValue("isIdle", true);
            state.SetBoolValue("hasTarget", false);
            state.SetBoolValue("isNearTarget", false);
            state.SetBoolValue("isRetreating", false);
            state.SetBoolValue("isNearCaravan", false);
            state.SetBoolValue("reachedBase", false);
        }
    }
}

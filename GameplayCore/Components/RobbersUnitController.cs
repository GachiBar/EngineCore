using GameplayCore.AI;
using GameplayCore.Mathematics;
using GameplayCore.Resources;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameplayCore.Components
{
    public partial class RobbersUnitController : Component
    {
        public int maxHealth;
        public int health;
        public int damage;
        public float speed;
        private RobberType robberTypeVal;
        public RobberType RobberTypeVal { get { return robberTypeVal; }  set { robberTypeVal = value; } }

        public RobbersUnitController() { }

        public void setDefaults(RobberType type)
        {
            this.robberTypeVal = type;
            var brains = GameObject.AddComponent<BrainComponent>();
            brains.Arbitrator = null;
            brains.ActionsAsset = null;
            switch (type)
            {
                case RobberType.WARRIOR:
                    maxHealth = 3;
                    health = 3;
                    damage = 1;
                    speed = 5;
                    //brains.Arbitrator = new RobberArbitrator();
                    break;
                case RobberType. WORKER:
                    maxHealth = 1;
                    health = 1;
                    damage = 0;
                    speed = 10;
                    brains.Arbitrator = new RobberArbitrator();
                    brains.ActionsAsset = new AIActionsAsset("Assets/AI/ai-worker.ai");
                    brains.ActionsAsset.Load();
                    brains.Arbitrator.InitState(brains._state);
                    //brains.ActionsAsset = 
                    break;
            }
        }

        public void MoveInDirection(Vector3 target)
        {
            var transform = GameObject.GetComponent<TransformComponent>();
            var rigidbody = GameObject.GetComponent<RigidbodyComponent>();

            if (transform == null)
            {
                Log.PrintError($"GameObject: {GameObject.Name} does not contains {nameof(TransformComponent)}");
                return;
            }
            if (rigidbody == null)
            {
                Log.PrintError($"GameObject: {GameObject.Name} does not contains {nameof(RigidbodyComponent)}");
                return;
            }
            Vector3 direction = - (new Vector3(transform.Position.X, 0, transform.Position.Z) - new Vector3(target.X, 0, target.Z));
            
            var velocity = rigidbody.Velocity;
            velocity += direction * speed;
            //Gizmos.DrawLine(transform.Position, velocity, new Vector3(255,255,255));
            //Gizmos.DrawLine(transform.Position, direction, new Vector3(0,0,255));
            
            rigidbody.Velocity = velocity;
        }  
    }
}

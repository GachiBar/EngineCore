using GameplayCore.AI;
using GameplayCore.Mathematics;
using GameplayCore.Resources;
using System;
using System.Collections.Generic;
using System.ComponentModel.Design;
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
        public float speedMul = 1;
        public float attackCooldown = 1.1f;
        private MaterialAsset remainWounds1 = null;
        private MaterialAsset remainWounds2 = null;
        private GameObject target = null;

        private RobberType robberTypeVal;
        public RobberType RobberTypeVal { get { return robberTypeVal; }  set { robberTypeVal = value; } }
        public GameObject TargetGO { get { return target; } set { target = value; } }

        public event Action<GameObject, RobbersUnitController> Dead;

        protected override void OnAttach(GameObject gameObject)
        {
            Dead += gameObject.Scene.First<GameObject>(go => go.GetComponent<CaravanRobController>() != null)
                .GetComponent<CaravanRobController>().killUnit;
        }

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
                    speed = 2.5f;
                    brains.Arbitrator = new WarriorArbitrator();
                    brains.ActionsAsset = new AIActionsAsset("Assets/AI/ai-warrior.ai");
                    brains.ActionsAsset.Load();
                    brains.Arbitrator.InitState(brains._state);
                    break;
                case RobberType. WORKER:
                    maxHealth = 1;
                    health = 1;
                    damage = 0;
                    speed = 2f;
                    brains.Arbitrator = new RobberArbitrator();
                    brains.ActionsAsset = new AIActionsAsset("Assets/AI/ai-worker.ai");
                    brains.ActionsAsset.Load();
                    brains.Arbitrator.InitState(brains._state);
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
            direction.Normalize();
            var velocity = rigidbody.Velocity;
            velocity = direction * speed * speedMul;
            //Gizmos.DrawLine(transform.Position, velocity, new Vector3(255,255,255));
            //Gizmos.DrawLine(transform.Position, direction, new Vector3(0,0,255));
            rigidbody.Velocity = velocity;
        }

        public override void Update()
        {
            if (health == 0)
                Dead?.Invoke(GameObject, this);
            if(RobberTypeVal == RobberType.WARRIOR && health != maxHealth)
            {
                var mesh = GameObject.GetComponent<MeshRenderComponent>();
                if (health == 2)
                    mesh.MaterialAsset = remainWounds2;
                if(health == 1)
                    mesh.MaterialAsset = remainWounds1;
            }
            if (target == null && RobberTypeVal == RobberType.WARRIOR)
            {
                target = findClosestGuard();
                if (target != null)
                {
                    Console.WriteLine(target.Name);
                    target.GetComponent<GuardComponent>().Dead += clearTarget;
                }
            }
        }

        private GameObject findClosestGuard()
        {
            GameObject controller = GameObject.Scene.First<GameObject>(go => go.GetComponent<CaravanRobController>() != null);
            var guards = controller.GetComponent<CaravanRobController>().getGuards();
            if (!(guards.Count() > 0)) return null;
            GameObject closest = guards.ToList().First<GameObject>();

            foreach (var unit in guards)
            {
                if (distanceTo(GameObject, closest) > distanceTo(GameObject, unit))
                {
                    closest = unit;
                }
            }
            return closest;
        }

        public void setMaterial1W(MaterialAsset ma) {
            remainWounds1 = ma;
        }
        public void setMaterial2W(MaterialAsset ma)
        {
            remainWounds2 = ma;
        }

        public void takeDamage(int damage)
        {
            if (health - damage < 0) health = 0;
            else health -= damage;
        }

        private float distanceTo(GameObject unit, GameObject target)
        {
            return (getPosProjectionXZ(unit) - getPosProjectionXZ(target)).Length();
        }

        private Vector3 getPosProjectionXZ(GameObject go)
        {
            var pos = go.GetComponent<TransformComponent>().Position;
            return new Vector3(pos.X, 0, pos.Z);
        }

        private void clearTarget(GameObject go, GuardComponent c)
        {
            target = null;
            DeleteGameObject(go);
        }
    }
}

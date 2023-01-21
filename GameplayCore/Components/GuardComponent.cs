using GameplayCore.AI;
using GameplayCore.Mathematics;
using GameplayCore.Resources;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static GameplayCore.Components.RobbersUnitController;

namespace GameplayCore.Components
{
    public class GuardComponent : Component
    {
        private MaterialAsset material = null;
        private MaterialAsset material2W = null;
        private MaterialAsset material1W = null;
        private GameObject target = null;

        public GameObject TargetGO { get { return target; } set { target = value; } }

        public int health;
        public float attackCooldown = 2f;
        public float speed = 2;

        public event Action<GameObject, GuardComponent> Dead;
        protected override void OnAttach(GameObject gameObject)
        {
            Dead += gameObject.Scene.First<GameObject>(go => go.GetComponent<CaravanRobController>() != null)
                .GetComponent<CaravanRobController>().killGuard;
        }
        public override void Update()
        {
            if (health == 0)
                Dead?.Invoke(GameObject, this);
            if (target == null)
            {
                target = findClosestRobber();
                if (target != null)
                {
                    Console.WriteLine(target.Name);
                    target.GetComponent<RobbersUnitController>().Dead += clearTarget;
                }
            }
            var mesh = GameObject.GetComponent<MeshRenderComponent>();
            if(health == 3)
                mesh.MaterialAsset = material;
            if (health == 2)
                mesh.MaterialAsset = material2W;
            if (health == 1)
                mesh.MaterialAsset = material1W;

        }

        private void clearTarget(GameObject go, RobbersUnitController c) {
            target = null;
            DeleteGameObject(go);
        }

        public void setDefaults(int hp)
        {
            var gameController = GameObject.Scene.First<GameObject>(go => go.GetComponent<CaravanRobController>() != null).GetComponent<CaravanRobController>();
            material = gameController.guardMaterial;
            material1W = gameController.guard1Material;
            material2W = gameController.guard2Material;
            var brains = GameObject.AddComponent<BrainComponent>();
            health = hp;
            brains.Arbitrator = null;
            brains.ActionsAsset = null;
            brains.Arbitrator = new GuardArbitrator();
            brains.ActionsAsset = new AIActionsAsset("Assets/AI/ai-guard.ai");
            brains.ActionsAsset.Load();
            brains.Arbitrator.InitState(brains._state);
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
            Vector3 direction = -(new Vector3(transform.Position.X, 0, transform.Position.Z) - new Vector3(target.X, 0, target.Z));
            direction.Normalize();
            var velocity = rigidbody.Velocity;
            velocity = direction * speed;
            //Gizmos.DrawLine(transform.Position, velocity, new Vector3(255,255,255));
            //Gizmos.DrawLine(transform.Position, direction, new Vector3(0,0,255));
            rigidbody.Velocity = velocity;
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

        private GameObject findClosestRobber()
        {
            GameObject controller = GameObject.Scene.First<GameObject>(go => go.GetComponent<CaravanRobController>() != null);
            var robbers = controller.GetComponent<CaravanRobController>().getRobbers();
            if (!(robbers.Count() > 0)) return null;
            GameObject closest = robbers.ToList().First<GameObject>();
            
            foreach (var unit in robbers)
            {
                if (distanceTo(GameObject, closest) > distanceTo(GameObject, unit))
                {
                    closest = unit;
                }
            }
            return closest;
        }

        public void takeDamage(int damage)
        {
            if (health - damage < 0) health = 0;
            else health -= damage;
        }
    }
}

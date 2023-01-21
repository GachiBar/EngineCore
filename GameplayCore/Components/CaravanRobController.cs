using GameplayCore.Resources;
using GameplayCore.Mathematics;
using System;
using System.Collections.Generic;
using System.Linq;
using static GameplayCore.Components.RobbersUnitController.RobberType;
using static GameplayCore.Components.RobbersUnitController;
using System.Security.Cryptography;
using ImGuiNET;
using GameplayCore.AI.CaravanAI.Actions;
using System.Xml.Linq;

namespace GameplayCore.Components
{
    public class CaravanRobController : Component
    {
        private const int gridStep = 2;
        private float _timer;
        private float _timer2;
        private GameObject _caravanHead;
        private Vector3 _direction;
        private Vector3 _prevDirection;
        private int _caravanStepTotal = 13;
        private int _caravanStepCounter = 0;
        private List<GameObject> _body;
        private List<GameObject> robbers;
        private List<GameObject> guards;
        private int gold = 15;
        private int score = 0;
        private int totalWorkers = 0;
        private int totalWarriors = 0;
        private int totalGuards = 0;
        private bool _isGameOver = false;
        private bool isEnhanceActive = false;
        private float enhanceTimer = 15;
        private string enhancmentName = "";
        private float enhanceDuration = 15;
        private float awareRadiusCaravan = 6;
        private bool isCaravanAttacked = false;
        private bool needToGetToBase = false;
        private int maxGuards = 10;

        public int FieldSizeX = 11;
        public int FieldSizeZ = 11;
        public MeshAsset cubeMesh;
        public MaterialAsset guardMaterial;
        public MaterialAsset guard1Material;
        public MaterialAsset guard2Material;
        public MaterialAsset caravanMaterial;
        public MaterialAsset grassMaterial;
        public MaterialAsset roadMaterial;
        public MaterialAsset blockpostMaterial;
        public MaterialAsset robberMaterial;
        public MaterialAsset robber1Material;
        public MaterialAsset robber2Material;
        public MaterialAsset workerMaterial;
        
        public int caravanSize = 5;
        public float caravanSpeed = 2.5f;
        public float caravanReachTimer = 25.0f;
        public int robberCost = 1;
        public int workerCost = 0;

        public int robberReturnedPts = 5;
        public int robberReturnedGold = 1;
        public int workerReturnedPts = 3;
        public int workerReturnedGold = 1;
        public int robberDiedPenalty = 3;
        public int workerDiedPenalty = 10;
        public int goldBringAmount = 2;

        public override void Initialize()
        {
            BuildMap();
            _caravanHead = CreateCaravan("head");
            _timer = 0;
            _timer2 = caravanReachTimer;
            _direction = Vector3.Right;
            _body = new List<GameObject>();
            while (_body.Count < caravanSize - 1)
            {
                var newSegment = CreateCaravan("body"+_body.Count);
                _body.Add(newSegment);
            }
            robbers = new List<GameObject>();
            guards = new List<GameObject>();
        }

        private void BuildMap() {
            int x = 0, z = 0;
            var tempPos = new Vector3(x, 0, z) * gridStep;
            var tempMater = grassMaterial;
            int i = 0;
            for (x = 1; x <= FieldSizeX-1; x++)
            {
                for(z = 1; z <= FieldSizeZ-1; z++)
                {
                    tempPos = new Vector3(x, 0, z) * gridStep;
                    if ((x == 1 || x == 14) && (z == 1 || z == 14))
                    {
                        var blockpost = InstantiateFieldCube(tempPos, blockpostMaterial, "blockpost_" + x + "_" + z);
                        blockpost.Name = "blockpost_" + i;
                        blockpost.AddComponent<BlockpostComponent>();
                        i++;
                    }
                    else if ((z == 1 || z == 14) && (x > 1 && x < 14))
                        InstantiateRoad(tempPos);
                    else if (x == 14 && z > 1 && z < 14)
                        InstantiateRoad(tempPos);
                    else if (x == 1 && z > 1 && z < 14)
                        InstantiateRoad(tempPos);
                }
            }
        }

        private GameObject InstantiateFieldCube(Vector3 pos, MaterialAsset material, string name)
        {
            var cube = CreateGameObject();
            var transform = cube.AddComponent<TransformComponent>();
            var mesh = cube.AddComponent<MeshRenderComponent>();
            cube.Name = name;
            transform.Position= pos;
            mesh.MaterialAsset = material;
            mesh.MeshAsset = cubeMesh;
            return cube;
        }

        private GameObject InstantiateRoad(Vector3 pos)
        {
            return InstantiateFieldCube(pos, roadMaterial, "road_" + pos.X + "_" + pos.Z);
        }

        private GameObject CreateCaravan(string name)
        {
            var caravan = CreateGameObject();
            var mesh = caravan.AddComponent<MeshRenderComponent>();
            var transform = caravan.AddComponent<TransformComponent>();
            transform.Position = new Vector3(2, 1.51f, 2);
            transform.Scale = Vector3.One * 0.85f;
            mesh.MaterialAsset = caravanMaterial;
            mesh.MeshAsset = cubeMesh;
            caravan.Name = name;

            return caravan;
        }

        public override void Update()
        {
            _isGameOver = !checkGameIsRunning();
            if (isEnhanceActive) {
                enhanceTimer -= Time.DeltaTime;
                if (enhanceTimer <= 0) { 
                    isEnhanceActive = false;
                    enhanceTimer = enhanceDuration;
                    foreach(var item in robbers)
                    {
                        var controller = item.GetComponent<RobbersUnitController>();
                        controller.speedMul= 1.0f;  
                        if(controller.RobberTypeVal == WARRIOR)
                        {
                            controller.damage = 1; 
                        } 
                    }
                }
            }
            if (_isGameOver)
            {
                Console.WriteLine("Total score: {0}", score);
                return;
            }
            if (!isCaravanAttacked)
            {
                _timer -= Time.DeltaTime;
                if (_timer <= 0)
                {
                    MoveCaravan();
                    _timer = caravanSpeed;
                }
            }
            else {
                _timer = caravanSpeed;
            }
            if (needToGetToBase) {
                _timer2 -= Time.DeltaTime;
                if (_timer2 <= 0)
                {
                    callGuards();
                    _timer2 = caravanReachTimer;
                }
            } 
            checkSpawnUnits();
            if (areRobbersNearCaravan())
            {
                isCaravanAttacked = true;
                needToGetToBase = true;
            }
            else isCaravanAttacked = false;
        }

        public override void RenderGUI()
        {
            GameObject unitToSpawn;
            ImGui.SetNextWindowBgAlpha(0.65f);
            ImGui.BeginChild("Info", new System.Numerics.Vector2(200, 150), true);
            if (!isCaravanAttacked && !needToGetToBase)
            {
                ImGui.Text("Caravan move in:");
                var progress = 1 - _timer / caravanSpeed;
                ImGui.ProgressBar(progress, new System.Numerics.Vector2(-1.0f, 5.0f), "");
                //Console.WriteLine(progress);
            } else
            {
                ImGui.TextColored(new System.Numerics.Vector4(255, 0, 0, 255), "Guards will arrive in:");
                var progress2 = 1 - _timer2 / caravanReachTimer;
                ImGui.ProgressBar(progress2, new System.Numerics.Vector2(-1.0f, 5.0f), "");
            }
            ImGui.Separator();
            if(_isGameOver && score < 1000)
            ImGui.TextColored(new System.Numerics.Vector4(255, 0, 0, 255), "GAME LOST");
            else ImGui.Text(String.Format("Gold: {0}", gold));
            if(score < 1000)
            ImGui.Text(String.Format("Total Score: {0}", score));
            else
                ImGui.TextColored(new System.Numerics.Vector4(0, 255, 0, 255), "GAME WON");
            ImGui.Separator();
            ImGui.TextColored(new System.Numerics.Vector4(0, 255, 0, 255), "Active units: ");
            ImGui.Text(String.Format("Thieves: {0}", totalWorkers));
            ImGui.Text(String.Format("Murderers: {0}", totalWarriors));
            ImGui.TextColored(new System.Numerics.Vector4(255, 0, 0, 255), String.Format("Guards: {0}", totalGuards));
            ImGui.EndChild();

            ImGui.SetNextWindowBgAlpha(0.65f);
            //ImGui.SetNextWindowPos(new System.Numerics.Vector2(500,500), ImGuiCond.Once);
            ImGui.BeginChild("Actions", new System.Numerics.Vector2(125, 170), true);

            ImGui.TextColored(new System.Numerics.Vector4(0, 255, 0, 255), "Spawn units: ");
            if (gold > 0)
                if (ImGui.Button("Spawn Thief"))
                { 
                    unitToSpawn = spawnUnitBase(workerMaterial, 0.45f, "worker", WORKER);
                    if (unitToSpawn != null)
                    {
                        totalWorkers++;
                        robbers.Add(unitToSpawn);
                        gold--;
                    }
                }
            if (gold > 1)
                if (ImGui.Button("Spawn Murderer"))
                {
                    unitToSpawn = spawnUnitBase(robberMaterial, 0.6f, "robber", WARRIOR);
                    if (unitToSpawn != null)
                    {
                        totalWarriors++;
                        robbers.Add(unitToSpawn);
                        gold -= 2;
                    }
                }
            if (ImGui.Button("Retreat!")) {
                callUnitsBack();
            }


            ImGui.Separator();
            ImGui.TextColored(new System.Numerics.Vector4(0, 255, 255, 255), "Enhancements: ");
            if (!isEnhanceActive)
            {
                if (gold < 3)
                    ImGui.PushStyleColor(ImGuiCol.Button, new System.Numerics.Vector4(255, 0, 0, 255));
                if (ImGui.Button("Agility") && gold > 2)
                {
                    enhancmentName = "Agility";
                    isEnhanceActive = true;
                    foreach (var item in robbers)
                    {
                        var controller = item.GetComponent<RobbersUnitController>();
                        if (controller.RobberTypeVal == WORKER)
                            controller.speedMul = 1.5f;
                        else controller.speedMul = 0.8f;
                    }
                }
                if (ImGui.Button("Strength") && gold > 2)
                {
                    enhancmentName = "Strength";
                    isEnhanceActive= true;
                    foreach (var item in robbers)
                    {
                        var controller = item.GetComponent<RobbersUnitController>();
                        if (controller.RobberTypeVal == WORKER)
                            controller.speedMul = 0.7f;
                        else controller.damage = 2;
                    }
                }
                if (gold < 3)
                    ImGui.PopStyleColor();
            }
            else {
                ImGui.TextColored(new System.Numerics.Vector4(255, 255, 100, 255), String.Format("{0} active:", enhancmentName));
                var progressE = 1 - enhanceTimer / enhanceDuration;
                ImGui.ProgressBar(progressE, new System.Numerics.Vector2(-1.0f, 10.0f), "");
            }
            ImGui.EndChild();
        }

        private void MoveCaravan() {
            var _headTransform = _caravanHead.GetComponent<TransformComponent>();
            var tail = _body[0];
            _body.RemoveAt(0);
            _body.Add(tail);
            var tailTransform = tail.GetComponent<TransformComponent>();
            tailTransform.Position = _headTransform.Position;
            _headTransform.Position += _direction * gridStep;
            _caravanStepCounter++;
            if (_caravanStepCounter == _caravanStepTotal) { 
                _caravanStepCounter = 0;
                _prevDirection = _direction;
                float cs = (float)Math.Cos(Math.PI /2);
                float sn = (float)Math.Sin(Math.PI/2);
                float x = _direction.X  * cs - _direction.Z * sn;
                float y = _direction.X * sn + _direction.Z * cs;
                _direction = new Vector3(x, _direction.Y, y);
                needToGetToBase = false;
                Random r = new Random();
                var res = r.Next(2) + 2;
                spawnGuards(res, 2);
            }
        }

        private void checkSpawnUnits() {
            GameObject unitToSpawn;
            if (Input.IsPressed(Keys.Q))
            {
                unitToSpawn = spawnUnitBase(workerMaterial, 0.45f, "worker", WORKER);
                if (unitToSpawn != null)
                {
                    totalWorkers++;
                    robbers.Add(unitToSpawn);
                    gold--;
                }
                else return;
            }
            if (Input.IsPressed(Keys.E)) {
                unitToSpawn = spawnUnitBase(robberMaterial, 0.6f, "robber", WARRIOR);
                if (unitToSpawn != null)
                {
                    totalWarriors++;
                    robbers.Add(unitToSpawn);
                    gold -= 2;
                }
                else return;
            }
            if (Input.IsPressed(Keys.BackSpace))
            {
                spawnGuards(1, 3);
            }
        }

        private GameObject spawnUnitBase(MaterialAsset material, float scale, String name, RobberType type) {
            if (!(gold > 0 && type == WORKER || gold > 1 && type == WARRIOR))
                return null;

            var unit = CreateGameObject();
            var transform = unit.AddComponent<TransformComponent>();
            var ruController = unit.AddComponent<RobbersUnitController>();
            var mesh = unit.AddComponent<MeshRenderComponent>();
            var rb = unit.AddComponent<RigidbodyComponent>();
            rb.IsKinematic = true;

            mesh.MeshAsset = cubeMesh;
            mesh.MaterialAsset = material;
            var baseObject = unit.Scene.First(go => go.GetComponent<BaseComponent>() != null);
            var baseTransform = baseObject.GetComponent<TransformComponent>();

            transform.Position = new Vector3(baseTransform.Position.X, 1.51f, baseTransform.Position.Z);
            //Debug spawn unit
            //transform.Position = new Vector3(2, 1.51f, 2);

            transform.Scale = Vector3.One * scale;

            unit.Name = name;
            ruController.setDefaults(type);
            ruController.setMaterial1W(robber1Material);
            ruController.setMaterial2W(robber2Material);
            return unit;
        }

        private bool areRobbersNearCaravan() { 
            var caravanParts = new List<GameObject>();
            caravanParts.Add(_caravanHead);
            caravanParts.AddRange(_body);
            Vector3 partPos;
            foreach(var part in caravanParts)
            {
                partPos = getPosProjectionXZ(part);
                Gizmos.DrawSphere(partPos, awareRadiusCaravan, new Vector3(255, 255, 255));
                foreach(var robber in robbers)
                {
                    if (calcDistance(partPos, getPosProjectionXZ(robber)) <= awareRadiusCaravan)
                        return true;
                }
            }
            return false;
        }

        private float calcDistance(Vector3 start, Vector3 target) {
            return (start - target).Length();
        }

        private Vector3 getPosProjectionXZ(GameObject go) { 
            var pos = go.GetComponent<TransformComponent>().Position;
            return new Vector3(pos.X, 0, pos.Z);
        }

        private void callUnitsBack()
        {
            foreach(var robber in robbers)
            {
                robber.GetComponent<BrainComponent>()._state.SetBoolValue("reachedBase", false);
                robber.GetComponent<BrainComponent>()._state.SetBoolValue("isRetreating", true);
            }
        }

        private bool checkGameIsRunning() {
            return (robbers.Count > 0 || gold > 0) && score < 1000;
        }

        private void callGuards() {
            spawnGuards(4, 3);
        }

        public void unitReturnToBase(GameObject unit) {
            Console.WriteLine("Ret");
            var controller = unit.GetComponent<RobbersUnitController>();
            switch (controller.RobberTypeVal) {
                case WARRIOR:
                    score += robberReturnedPts;
                    gold += robberReturnedGold;
                    totalWarriors--;
                    break;
                case WORKER:
                    score += workerReturnedPts;
                    gold += workerReturnedGold;
                    totalWorkers--;
                    break;
            }
            robbers.Remove(unit);
            DeleteGameObject(unit);
            foreach (var guard in guards)
            {
                guard.GetComponent<GuardComponent>().TargetGO = null;
            }
        }

        public GameObject getCaravanPosition()
        {
            Random r = new Random();
            int c = r.Next(_body.Count) - 1;
            Console.WriteLine(c + ":", false);
            if (c < 0)
                return _caravanHead;
            else return _body[c];
        }

        public void addGold() {
            gold += goldBringAmount;
            score += 10;
        }

        public void killUnit(GameObject killedUnit, RobbersUnitController controller)
        {
            switch (controller.RobberTypeVal)
            {
                case WARRIOR: score -= robberDiedPenalty; totalWarriors--; break;
                case WORKER: score -= workerDiedPenalty; totalWorkers--;  break; 
            }
            robbers.Remove(killedUnit);
            DeleteGameObject(killedUnit);
            foreach(var guard in guards)
            {
                guard.GetComponent<GuardComponent>().TargetGO = null;
            }
        }

        public void killGuard(GameObject killedUnit, GuardComponent controller) {
            totalGuards--;
            guards.Remove(killedUnit);
            DeleteGameObject(killedUnit);
            foreach (var robber in robbers)
            {
                if(robber.GetComponent<RobbersUnitController>().RobberTypeVal == WARRIOR)
                    robber.GetComponent<RobbersUnitController>().TargetGO = null;
            }
        }

        public void spawnGuards(int count, int hp) {
            Random r = new Random();
            var locC = count;
            while (totalGuards < maxGuards && locC > 0)
            {
                var guard = CreateGameObject();
                var gController = guard.AddComponent<GuardComponent>();
                var gRB = guard.AddComponent<RigidbodyComponent>();
                var gTransform = guard.AddComponent<TransformComponent>();
                var mesh = guard.AddComponent<MeshRenderComponent>();
                gRB.IsKinematic = true;

                mesh.MeshAsset = cubeMesh;
                if (hp == 2)
                    mesh.MaterialAsset = guard2Material;
                if (hp == 3)
                    mesh.MaterialAsset = guardMaterial;

                var baseObjects = guard.Scene.OfType<GameObject>().Where(go => go.GetComponent<BlockpostComponent>() != null);
                var baseObject = baseObjects.ToList<GameObject>()[0];
                foreach (var baseTile in baseObjects)
                {
                    if (distanceTo(_caravanHead, baseObject) > distanceTo(_caravanHead, baseTile))
                    {
                        baseObject = baseTile;
                    }
                }
                var baseTransform = baseObject.GetComponent<TransformComponent>();
                gTransform.Position = new Vector3(baseTransform.Position.X + r.Next(2) * 0.33f, 1.51f, baseTransform.Position.Z * r.Next(3) * 0.12f);
                //Debug spawn unit
                //transform.Position = new Vector3(2, 1.51f, 2);

                gTransform.Scale = Vector3.One * 0.66f;

                guard.Name = "guard";
                gController.setDefaults(hp);
                totalGuards++;
                locC--;
                guards.Add(guard);
            }
        }

        public List<GameObject> getRobbers() { return robbers; }
        public List<GameObject> getGuards() { return guards; }

        private float distanceTo(GameObject unit, GameObject target)
        {
            return (getPosProjectionXZ(unit) - getPosProjectionXZ(target)).Length();
        }
    }
}

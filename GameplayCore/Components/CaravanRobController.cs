using GameplayCore.Resources;
using GameplayCore.Mathematics;
using System;
using System.Collections.Generic;
using System.Linq;
using static GameplayCore.Components.RobbersUnitController.RobberType;
using static GameplayCore.Components.RobbersUnitController;
using System.Security.Cryptography;
using ImGuiNET;



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
        private int gold = 15;
        private int score = 0;
        private int totalWorkers = 0;
        private int totalWarriors = 0;
        private bool _isGameOver = false;
        private bool isEnhanceActive = false;
        private float enhanceTimer = 15;
        private string enhancmentName = "";
        private float enhanceDuration = 15;
        private float awareRadiusCaravan = 6;
        private bool isCaravanAttacked = false;
        private bool needToGetToBase = false;

        public int FieldSizeX = 11;
        public int FieldSizeZ = 11;
        public MeshAsset cubeMesh;
        public MaterialAsset guardMaterial;
        public MaterialAsset caravanMaterial;
        public MaterialAsset grassMaterial;
        public MaterialAsset roadMaterial;
        public MaterialAsset blockpostMaterial;
        public MaterialAsset robberMaterial;
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


        public override void Initialize()
        {
            BuildMap();
            _caravanHead = CreateCaravan("head");
            _timer = 0;
            _direction = Vector3.Right;
            _body = new List<GameObject>();
            while (_body.Count < caravanSize - 1)
            {
                var newSegment = CreateCaravan("body"+_body.Count);
                _body.Add(newSegment);
            }
            robbers = new List<GameObject>();
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
                if (!needToGetToBase) { 
                    _timer2= caravanReachTimer;
                }
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
                        //TODO warriors = 1hp, speed of workers++
                    }
                }
                if (ImGui.Button("Strength") && gold > 2)
                {
                    enhancmentName = "Strength";
                    isEnhanceActive= true;
                    foreach (var item in robbers)
                    {
                        //TODO warr dmg +1, speed of workers--
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
                deleteAllUnits();
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
            //TODO: AI to go back; give gold back and small pts
        }

        private bool checkGameIsRunning() {
            return (robbers.Count > 0 || gold > 0) && score < 1000;
        }

        private void callGuards() {
            Console.WriteLine("GUARDS!!!");
            //TODO Spawn guards with more HP
        }

        private void deleteAllUnits() { 
            foreach(var unit in robbers)
            {
                unit.Scene.DeleteGameObject(unit);
            }
            robbers.RemoveAll(go => true);
            Console.WriteLine("deleted: {0} units in list", robbers.Count);
            totalWorkers = 0;
            totalWarriors= 0;
        }

        public void unitReturnToBase(GameObject unit) {
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
            unit.Scene.DeleteGameObject(unit);
        }

        public GameObject getCaravanPosition()
        {
            return _caravanHead;
            //TODO do random part
        }
    }
}

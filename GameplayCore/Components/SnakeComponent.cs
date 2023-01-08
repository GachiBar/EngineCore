using GameplayCore.Mathematics;
using GameplayCore.Resources;
using System;
using System.Collections.Generic;

namespace GameplayCore.Components
{
    public class SnakeComponent : Component
    {
        private const int GridStep = 2;
        private const float Epsilon = 0.5f;

        private bool _isGameOver;
        private int _score;
        private float _timer;
        private Vector3 _direction;
        private Vector3 _prevDirection;
        private Random _random;
        private GameObject _head;
        private GameObject _berry;
        private List<GameObject> _body;

        public float Timeout = 0.5f;
        public MeshAsset CubeMesh;
        public MaterialAsset GrayMaterial;
        public MaterialAsset GreenLightMaterial;
        public MaterialAsset GreenDarkMaterial;
        public MaterialAsset BlueMaterial;
        public MaterialAsset RedMaterial;
        
        public int FieldSize = 10;

        public SnakeComponent()
        {
            _isGameOver = false;
            _score = 3;
            _timer = 0;
            _direction = Vector3.UnitX;
            _prevDirection = _direction;
            _random = new Random();
            _body = new List<GameObject>();            
        }

        public override void Initialize()
        {
            BuildMap();
            float halfSize = GridStep * (FieldSize / 2);
            _head = InstantiateCube(new Vector3(halfSize, GridStep, halfSize), BlueMaterial);
            _berry = InstantiateCube(GetBerryRandomPosition(), RedMaterial);
        }

        public override void Update()
        {
            if (_isGameOver)
            {
                return;
            }

            _timer -= Time.DeltaTime;
            _direction = GetDirection();

            if (_timer <= 0)
            {                
                UpdateGameWorld();
                _prevDirection = _direction;
                _timer = Timeout;
            }
        }

        public override void Render()
        {
            foreach (var segment in _body)
            {
                var transform = segment.GetComponent<TransformComponent>();
                Gizmos.DrawSphere(transform.ModelMatrix, 0.5f, new Vector3(0.0f, 255.0f, 0.0f));
            }
            if (_berry == null) return;

            var berryTransform = _berry.GetComponent<TransformComponent>();
            Gizmos.DrawSphere(berryTransform.ModelMatrix, 0.5f, new Vector3(0.0f, 0.0f, 255.0f));
        }

        private void BuildMap()
        {
            // Create field.
            for (int x = 1; x <= FieldSize; x++)
            {
                for (int z = 1; z <= FieldSize; z++)
                {
                    var position = new Vector3(x, 0, z) * GridStep;
                    var material = ChooseMaterial(x + z);
                    InstantiateCube(position, material);
                }
            }

            // Create borders
            var topOffset = new Vector3(0, 0, 0);
            var bottomOffset = new Vector3(0, 0, GridStep * (FieldSize + 1));

            for (int x = 1; x <= FieldSize; x++)
            {
                var offset = new Vector3(GridStep * x, 0, 0);
                InstantiateCube(offset + topOffset, GrayMaterial);
                InstantiateCube(offset + bottomOffset, GrayMaterial);
            }

            var leftOffset = new Vector3(GridStep * 0, 0, 0);
            var rightOffset = new Vector3(GridStep * (FieldSize + 1), 0, 0);

            for (int z = 0; z <= FieldSize + 1; z++)
            {
                var offset = new Vector3(0, 0, GridStep * z);
                InstantiateCube(offset + leftOffset, GrayMaterial);
                InstantiateCube(offset + rightOffset, GrayMaterial);
            }
        }

        private MaterialAsset ChooseMaterial(int index)
        {
            return index % 2 == 0 ? GreenDarkMaterial : GreenLightMaterial;
        }

        private Vector3 GetBerryRandomPosition()
        {
            var newX = GridStep * _random.Next(1, FieldSize);
            var newZ = GridStep * _random.Next(1, FieldSize);
            return new Vector3(newX, GridStep, newZ);
        }

        private GameObject InstantiateCube(Vector3 position, MaterialAsset material)
        {
            var cube = CreateGameObject();
            var transform = cube.AddComponent<TransformComponent>();
            var mesh = cube.AddComponent<MeshRenderComponent>();
            
            transform.Position = position;
            mesh.MeshAsset = CubeMesh;
            mesh.MaterialAsset = material;
            return cube;
        }

        private Vector3 GetDirection()
        {
            if (Input.WasJustPressed(Keys.W) && _prevDirection != Vector3.UnitZ)
            {
                return -Vector3.UnitZ;
            }
            if (Input.WasJustPressed(Keys.S) && _prevDirection != -Vector3.UnitZ)
            {
                return Vector3.UnitZ;
            }
            if (Input.WasJustPressed(Keys.A) && _prevDirection != Vector3.UnitX)
            {
                return -Vector3.UnitX;
            }
            if (Input.WasJustPressed(Keys.D) && _prevDirection != -Vector3.UnitX)
            {
                return Vector3.UnitX;
            }

            return _direction;
        }

        private void UpdateGameWorld()
        {
            _isGameOver |= IsCrashedIntoWall();
            _isGameOver |= IsCrashedIntoBody();

            if (_isGameOver)
            {
                return;
            }

            MoveSnake();

            if (TryEatBerry())
            {
                _score += 1;
            }
        }

        private bool IsCrashedIntoWall()
        {
            float size = GridStep * (FieldSize + 1);

            var transform = _head.GetComponent<TransformComponent>();

            return MathUtil.WithinEpsilon(transform.Position.X, 0, Epsilon)
                || MathUtil.WithinEpsilon(transform.Position.X, size, Epsilon)
                || MathUtil.WithinEpsilon(transform.Position.Z, 0, Epsilon)
                || MathUtil.WithinEpsilon(transform.Position.Z, size, Epsilon);
        }

        private bool IsCrashedIntoBody()
        {
            var headTransform = _head.GetComponent<TransformComponent>();

            foreach (var segment in _body)
            {
                var segmentTransform = segment.GetComponent<TransformComponent>();

                if (MathUtil.WithinEpsilon(headTransform.Position.X, segmentTransform.Position.X, Epsilon) &&
                    MathUtil.WithinEpsilon(headTransform.Position.Z, segmentTransform.Position.Z, Epsilon))
                {
                    return true;
                }
            }

            return false;
        }

        private bool TryEatBerry()
        {
            var headTransform = _head.GetComponent<TransformComponent>();
            var berryTransform = _berry.GetComponent<TransformComponent>();

            if (MathUtil.WithinEpsilon(headTransform.Position.X, berryTransform.Position.X, Epsilon) &&
                MathUtil.WithinEpsilon(headTransform.Position.Z, berryTransform.Position.Z, Epsilon))
            {
                berryTransform.Position = GetBerryRandomPosition();
                return true;
            }

            return false;
        }

        private void MoveSnake()
        {
            var headTransform = _head.GetComponent<TransformComponent>();

            if (_body.Count < _score)
            {
                var newSegment = InstantiateCube(headTransform.Position, BlueMaterial);
                _body.Add(newSegment);
            }
            else
            {
                var tail = _body[0];
                _body.RemoveAt(0);
                _body.Add(tail);
                var tailTransform = tail.GetComponent<TransformComponent>();
                tailTransform.Position = headTransform.Position;
            }

            headTransform.Position += _direction * GridStep;
        }
    }
}

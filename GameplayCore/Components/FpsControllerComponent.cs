using GameplayCore.Mathematics;
using GameplayCore.Resources;
using ImGuiNET;
using System;

namespace GameplayCore.Components
{
    public class FpsControllerComponent : Component
    {
        private const float Weight = 0.03f;
        private const float BulletRadius = 0.3f;

        private RigidbodyComponent _rigidbodyComponent;
        private CapsuleCollisionComponent _capsuleCollisionComponent;

        private Vector3 _axis;
        private float _yaw = 0.0f;
        private float _pitch = 0.0f;
        private bool _isJump;
        
        public float Speed = 3.0f;
        public float JumpSpeed = 8.0f;
        public float Sensivity = 1.0f;
        public float BulletImpulse = 50.0f;
        public float BulletLifeTime = 2.0f;
        public MeshAsset BulletMesh;
        public GameObject Head;

        public override void Update()
        {         
            var headTransform = Head.GetComponent<TransformComponent>();

            if (headTransform != null)
            {
                var dx = Input.GetKeyValue(Keys.MouseX);
                var dy = Input.GetKeyValue(Keys.MouseY);

                _yaw -= dx * Weight * Sensivity;
                _pitch -= dy * Weight * Sensivity;
                _pitch = MathUtil.Clamp(_pitch, -MathUtil.PiOverTwo, MathUtil.PiOverTwo);
                _axis = GetAxis();

                var rotation = Quaternion.RotationYawPitchRoll(_yaw, _pitch, 0);
                headTransform.LocalRotation = rotation;

                if (Input.WasJustPressed(Keys.LeftMouseButton))
                {
                    ThrowBullet(headTransform.Position + headTransform.Forward * 0.5f, headTransform.Forward);
                }

                _isJump |= Input.WasJustPressed(Keys.SpaceBar);
            }            
        }

        public override void FixedUpdate()
        {
            if (_rigidbodyComponent != null)
            {               
                var headTransform = Head.GetComponent<TransformComponent>();

                if (headTransform != null)
                {
                    var rotation = Quaternion.RotationYawPitchRoll(_yaw, 0, 0);
                    var direction = Vector3.Transform(_axis, rotation);
                    direction.Normalize();
                    var yAxis = new Vector3(0.0f, _rigidbodyComponent.Velocity.Y, 0.0f);
                    _rigidbodyComponent.Velocity = direction * Speed + yAxis;
                }    

                if (IsGrounded() && _isJump)
                {
                    _rigidbodyComponent.Velocity += Vector3.Up * JumpSpeed;
                }
            }

            _isJump = false;
        }

        public override void RenderGUI()
        {
            ImGui.BeginChild("HUD");
            ImGui.Text("My HUD work!");
            ImGui.EndChild();
        }

        protected override void OnAttach(GameObject gameObject)
        {
            _rigidbodyComponent = gameObject.GetComponent<RigidbodyComponent>();
            _capsuleCollisionComponent = gameObject.GetComponent<CapsuleCollisionComponent>();
            
            gameObject.ComponentAdded += OnComponentAdded;
            gameObject.ComponentRemoved += OnComponentRemoved;
        }

        protected override void OnDetach(GameObject gameObject)
        {
            gameObject.ComponentAdded -= OnComponentAdded;
            gameObject.ComponentRemoved -= OnComponentRemoved;
        }

        private void OnComponentAdded(GameObject gameObject, Component component)
        {
            if (component is RigidbodyComponent rigidbodyComponent)
            {
                _rigidbodyComponent = rigidbodyComponent;                
            }
            if (component is CapsuleCollisionComponent capsuleCollisionComponent)
            {
                _capsuleCollisionComponent = capsuleCollisionComponent;
            }
        }

        private void OnComponentRemoved(GameObject gameObject, Component component)
        {
            if (component is RigidbodyComponent)
            {
                _rigidbodyComponent = null;
            }
            if (component is CapsuleCollisionComponent)
            {
                _capsuleCollisionComponent = null;
            }
        }

        private Vector3 GetAxis()
        {
            var direction = Vector3.Zero;

            if (Input.IsPressed(Keys.A))
            {
                direction -= Vector3.UnitX;
            }
            if (Input.IsPressed(Keys.D))
            {
                direction += Vector3.UnitX;
            }
            if (Input.IsPressed(Keys.W))
            {
                direction -= Vector3.UnitZ;
            }
            if (Input.IsPressed(Keys.S))
            {
                direction += Vector3.UnitZ;
            }

            return direction;
        }

        private void ThrowBullet(Vector3 position, Vector3 direction)
        {
            var bullet = GameObject.Scene.CreateGameObject();
            var bulletTransform = bullet.AddComponent<TransformComponent>();
            var bulletMesh = bullet.AddComponent<MeshRenderComponent>();
            var bulletRigidbody = bullet.AddComponent<RigidbodyComponent>();
            var bulletSphere = bullet.AddComponent<SphereCollisionComponent>();
            var bulletTimer = bullet.AddComponent<TimerDestroyerComponent>();
            var projectile = bullet.AddComponent<ProjectileComponent>();            

            bulletTransform.Position = position;
            bulletTransform.Scale = new Vector3(BulletRadius);
            bulletMesh.MeshAsset = BulletMesh;
            bulletSphere.Radius = BulletRadius;
            bulletTimer.Timer = BulletLifeTime;
            bulletRigidbody.Mass = 1.0f;
            bulletRigidbody.AddImpulse(direction * BulletImpulse);
        }

        private bool IsGrounded()
        {
            if (_capsuleCollisionComponent != null)
            {
                var transform = GameObject.GetComponent<TransformComponent>();

                if (transform != null)
                {
                    float capsuleHeight = _capsuleCollisionComponent.Height + _capsuleCollisionComponent.Radius;
                    float epsilon = 0.01f;
                    capsuleHeight += epsilon;

                    var origin = transform.Position + Vector3.Down * (capsuleHeight);
                    var direction = Vector3.Down * epsilon;
                    return Physics.CastRay(origin, direction, out var other);
                }
            }

            return false;
        }
    }
}

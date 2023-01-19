using GameplayCore.Mathematics;
using System;

namespace GameplayCore.Components
{
    public class CameraComponent : Component
    {
        private const int PlainPointsCount = 4;
        private readonly Vector3 FrustrumColor = new Vector3(255.0f, 255.0f, 255.0f);

        public float FieldOfView = 90;
        public float Near = 0.1f;
        public float Far = 100.0f;
        public float Zoom = 10.0f;
        public bool Orthographic;

        private static readonly Vector3[] NearPlain;
        private static readonly Vector3[] FarPlain;

        private TransformComponent _transformComponent = null;      
        
        static CameraComponent()
        {
            NearPlain = new Vector3[PlainPointsCount + 1]
            {
                new Vector3(0.1f, 0.1f, -0.1f),
                new Vector3(-0.1f, 0.1f, -0.1f),
                new Vector3(-0.1f, -0.1f, -0.1f),
                new Vector3(0.1f, -0.1f, -0.1f),
                new Vector3(0.1f, 0.1f, -0.1f),
            };

            FarPlain = new Vector3[PlainPointsCount + 1];
            Vector3 scale = new Vector3(2.0f, 2.0f, 4.0f);

            for (int i = 0; i < PlainPointsCount + 1; i++)
            {
                FarPlain[i] = NearPlain[i] * scale;
            }
        }

        public override void Update()
        {
            if (_transformComponent != null)
            {
                var fow = MathUtil.DegreesToRadians(FieldOfView);
                var eye = _transformComponent.Position;
                var target = _transformComponent.Position + _transformComponent.Forward;
                var up = _transformComponent.Up;
                var view = Matrix.LookAtRH(eye, target, up);

                if (Orthographic)
                {
                    var projection = Matrix.OrthoRH(Screen.Width / Zoom, Screen.Height / Zoom, Near, Far);
                    EngineApi.RenderApi.SetViewProjection(Time.EllapsedTime, view, projection, eye);
                }
                else
                {
                    float aspect = (float)Screen.Width / Screen.Height;
                    var projection = Matrix.PerspectiveFovRH(fow, aspect, Near, Far);
                    EngineApi.RenderApi.SetViewProjection(Time.EllapsedTime, view, projection, eye);
                }                
            }
        }

        public override void EditorRender()
        {
            if (_transformComponent != null)
            {
                var model = Matrix.Identity;
                model *= Matrix.RotationQuaternion(_transformComponent.Rotation);
                model *= Matrix.Translation(_transformComponent.Position);

                Gizmos.DrawSphere(model, 0.2f, FrustrumColor);
                Gizmos.DrawCurve(model, NearPlain, FrustrumColor);
                Gizmos.DrawCurve(model, FarPlain, FrustrumColor);

                for (int i = 0; i < PlainPointsCount; i++)
                {
                    Gizmos.DrawLine(model, NearPlain[i], FarPlain[i], FrustrumColor);
                }
            }
        }

        protected override void OnAttach(GameObject gameObject)
        {
            _transformComponent = gameObject.GetComponent<TransformComponent>();

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
            if (component is TransformComponent transformComponent)
            {
                _transformComponent = transformComponent;
            }
        }

        private void OnComponentRemoved(GameObject gameObject, Component component)
        {
            if (component is TransformComponent)
            {
                _transformComponent = null;
            }
        }
    }
}

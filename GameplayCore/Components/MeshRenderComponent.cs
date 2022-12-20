using GameplayCore.Resources;
using GameplayCore.Serialization;
using System;
using GameplayCore.EngineApi;

namespace GameplayCore.Components
{
    public class MeshRenderComponent : Component
    {
        private TransformComponent _transformComponent = null;
        [SerializeField] private MeshAsset _asset;
        [SerializeField] private MaterialAsset _material;

        public ulong Id = 1;
        private ulong _materialId = 0;

        public override void Initialize() 
        {
            if (_asset != null)
            {
                _asset.Load();
                Id = _asset.Id;
            }
            
            if (_material != null)
            {
                _material.Load();
            }
            
            if(debug)
                Debug();
        }

        private static bool debug = false;
        public void Debug()
        {
            debug = false;
            Console.WriteLine("Pull material");
            var data = RenderApi.PullMaterial(0);
            data.roughness.isTextured = true;
            data.roughness.id = 10;
            Console.WriteLine("Commit material");
            RenderApi.CommitMaterial(0, data);
        }

        public override void Render()
        {
            if (_transformComponent != null)
            {
                EngineApi.RenderApi.DrawModel(
                    Id,
                    _materialId, 
                    _transformComponent.ModelMatrix);
            }            
        }

        internal override void Invalidate(string fieldName)
        {
            base.Invalidate(fieldName);

            if (fieldName == "_asset")
            {
                if (_asset != null)
                {
                    _asset.Load();
                    Id = _asset.Id;
                }
                else
                {
                    Id = 0;
                }   
            }
            else if (fieldName == "_material")
            {
                if (_material != null)
                {
                    _material.Load();
                    _materialId = _material.Id;
                }
                else
                {
                    _materialId = 0;
                }
            }
        }

        protected override void OnAttach(GameObject gameObject)
        {
            _transformComponent = GameObject.GetComponent<TransformComponent>();

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

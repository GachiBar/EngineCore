using GameplayCore.Resources;
using GameplayCore.Serialization;
using System;

namespace GameplayCore.Components
{
    public class MeshRenderComponent : Component
    {
        private TransformComponent _transformComponent = null;
        [SerializeField] private MeshAsset _asset;
        //Material{Texture, Metalic, Raffness, Normal}

        public ulong Id = 1;
        public float Metallic = 0;
        public float Roughness = 0;

        public override void Initialize() 
        {
            if (_asset != null)
            {
                _asset.Load();
                Id = _asset.Id;
            }
        }

        public override void Render()
        {
            if (_transformComponent != null)
            {
                EngineApi.RenderApi.DrawModel(
                    Id, 
                    Metallic, 
                    Roughness, 
                    _transformComponent.ModelMatrix);
            }            
        }

        internal override void Invalidate(string fieldName)
        {
            base.Invalidate(fieldName);
            
            if(fieldName != "_asset") return;
            
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

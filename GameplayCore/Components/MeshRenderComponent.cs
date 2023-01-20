using GameplayCore.Editor;
using GameplayCore.Resources;
using GameplayCore.Serialization;
using System;

namespace GameplayCore.Components
{
    public class MeshRenderComponent : Component
    {
        private TransformComponent _transformComponent = null;

        [SerializeField]
        [InspectorName("MeshAsset")]
        private MeshAsset _meshAsset;

        [SerializeField]
        [InspectorName("MaterialAsset")]
        private MaterialAsset _materialAsset;

        private ulong _meshId = 0;
        private ulong _materialId = 0;

        public MeshAsset MeshAsset
        {
            get => _meshAsset;
            set
            {
                _meshAsset = value;
                TryReloadMesh();
            }
        }

        public MaterialAsset MaterialAsset
        {
            get => _materialAsset;
            set
            {
                _materialAsset = value;
                TryReloadMaterial();
            }
        }

        public override void Render()
        {
            if (_transformComponent == null)
            {
                Log.PrintError($"GameObject: {GameObject.Name} does not contains {nameof(TransformComponent)}");
                return;
            }

            EngineApi.RenderApi.DrawModel(
                _meshId,
                _materialId,
                BitConverter.ToUInt32(GameObject.Guid.ToByteArray(), 0),
                _transformComponent.ModelMatrix);
        }

        internal override void Invalidate(string fieldName)
        {
            switch (fieldName)
            {
                case nameof(_meshAsset):
                    TryReloadMesh();
                    break;
                case nameof(_materialAsset):
                    TryReloadMaterial();
                    break;
            }
        }

        protected override void OnAttach(GameObject gameObject)
        {
            TryReloadMesh();
            TryReloadMaterial();

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

        private bool TryReloadMesh()
        {
            if (_meshAsset != null)
            {
                _meshAsset.Load();
                _meshId = _meshAsset.Id;
                return true;
            }

            _meshId = 0;
            return false;
        }

        private bool TryReloadMaterial()
        {
            if (_materialAsset != null)
            {
                _materialAsset.Load();
                _materialId = _materialAsset.Id;
                return true;
            }

            _materialId = 0;
            return false;
        }
    }
}

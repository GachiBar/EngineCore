using System;
using GameplayCore.EngineApi;
using GameplayCore.Serialization;

namespace GameplayCore.Resources
{
    public class MaterialAsset : Resource
    {
        [SerializeField] private Material _material;
        private ulong _id;
        
        public ulong Id => _id;
        
        // Creates or changes material data by id to c++ side
        public MaterialAsset(string filepath, Guid? guid = null) : base(filepath, guid)
        {
        }

        public void Commit()
        {
            // Set
            RenderApi.CommitMaterial(_id, new Internal_Material(_material));
        }

        // Gets data from c++ side

        public void Pull()
        {
            RenderApi.PullMaterial(_id);
            // Why would you pull material from c++ side
            // TODO: Do something with dis or just remove method
        }

        public void Load()
        {
            if(_id != 0) return;

            ulong id = RenderApi.CalculateFreeMaterialId();
            _id = id;
            Commit();
        }
    }
}
using System;
using System.IO;
using GameplayCore.EngineApi;

namespace GameplayCore.Resources
{
    public class MeshAsset : Resource
    {
        private ulong _id = ulong.MaxValue;
        public ulong Id => _id;
        
        public MeshAsset(string filepath, Guid? guid = null) : base(filepath, guid)
        {
        }

        public void Load()
        {
            if (_id != ulong.MaxValue)
            {
                Console.WriteLine($"Load avoid: Current id is {_id}");
                return;
            }
            
            ulong id = RenderApi.CalculateFreeId();
            RenderApi.RegisterModel(id, FilePath);
            RenderApi.RegisterTexture(id, FilePath.Split('.')[0] + ".jpg");
            _id = id;
            
            Console.WriteLine($"Loaded new mesh with id {id} from {FilePath}");
        }
    }
}
using System;
using GameplayCore.EngineApi;

namespace GameplayCore.Resources
{
    public class TextureAsset : Resource
    {
        private ulong _id = ulong.MaxValue;
        public ulong Id => _id;
        
        public TextureAsset(string filepath, Guid? guid = null) : base(filepath, guid)
        {
        }
        
        public void Load()
        {
            if (_id != ulong.MaxValue)
            {
                Console.WriteLine($"Load avoid: Current id is {_id}");
                return;
            }
            
            ulong id = RenderApi.CalculateFreeTextureId();
            RenderApi.RegisterTexture(id, FilePath);
            _id = id;
            
            Console.WriteLine($"Loaded new texture with id {id} from {FilePath}");
        }
    }
}
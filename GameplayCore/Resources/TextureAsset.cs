using System;

namespace GameplayCore.Resources
{
    public class TextureAsset : Resource
    {
        public TextureAsset(string filepath, Guid? guid = null) : base(filepath, guid)
        {
        }
    }
}
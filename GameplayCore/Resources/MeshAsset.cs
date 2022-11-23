using System;

namespace GameplayCore.Resources
{
    public class MeshAsset : Resource
    {
        public MeshAsset(string filepath, Guid? guid = null) : base(filepath, guid)
        {
        }
    }
}
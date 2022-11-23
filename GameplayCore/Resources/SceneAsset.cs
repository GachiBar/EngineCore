using System;

namespace GameplayCore.Resources
{
    public class SceneAsset : Resource
    {
        public SceneAsset(string filepath, Guid? guid = null) : base(filepath, guid)
        {
        }
    }
}
﻿using System;
using GameplayCore.EngineApi;

namespace GameplayCore.Resources
{
    public class MeshAsset : Resource
    {
        private ulong _id;
        public ulong Id => _id;
        
        public MeshAsset(string filepath, Guid? guid = null) : base(filepath, guid)
        {
        }

        public void Load()
        {
            ulong id = RenderApi.CalculateFreeId();
            RenderApi.RegisterModel(id, FilePath);
            _id = id;
        }
    }
}
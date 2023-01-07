using System;
using System.Runtime.InteropServices;
using GameplayCore.Mathematics;

namespace GameplayCore.Resources
{
    [Serializable]
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public class Material
    {
        // DiffuseData   diffuseData;
        public Color4 diffuseColor;
        public TextureAsset diffuseTexture;

        // NormalData    normalData;
        public Color3 normalsColor;
        public TextureAsset normalsTexture;

        // RoughnessData roughnessData;
        public float roughnessValue;
        public TextureAsset roughnessTexture;

        // MetallicData  metallicData;
        public float metallicValue;
        public TextureAsset metallicTexture;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct TransferMaterial
    {
        public Color4 diffuseColor;
        public ulong diffuseTextureId;
        
        public Color3 normalsValue;
        public ulong normalsTextureId;
        
        public float roughnessValue;
        public ulong roughnessTextureId;
        
        public float metallicValue;
        public ulong metallicTextureId;

        public TransferMaterial(Material material)
        {
            diffuseColor = material.diffuseColor;
            normalsValue = material.normalsColor;
            roughnessValue = material.roughnessValue;
            metallicValue = material.metallicValue;

            diffuseTextureId = material.diffuseTexture?.Id ?? 0;
            normalsTextureId = material.normalsTexture?.Id ?? 0;
            roughnessTextureId = material.roughnessTexture?.Id ?? 0;
            metallicTextureId = material.metallicTexture?.Id ?? 0;
        }
    }
}
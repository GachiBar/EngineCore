using System;
using System.Runtime.InteropServices;
using GameplayCore.Mathematics;

namespace GameplayCore.Resources
{
    [Serializable]
    public class Material
    {
        // DiffuseData   diffuseData;
        public Color4 diffuse_color;
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
    public struct Internal_Material
    {
        public Internal_DiffuseData diffuse;
        public Internal_NormalData normals;
        public Internal_RoughnessData roughness;
        public Internal_MetallicData metallic;

        public Internal_Material(Material material)
        {
            diffuse = new Internal_DiffuseData();
            diffuse.isTextured = material.diffuseTexture != null && material.diffuseTexture.Id != ulong.MaxValue;
            if (diffuse.isTextured)
                diffuse.id = material.diffuseTexture.Id;
            else
                diffuse.diffuseColor = material.diffuse_color;

            normals = new Internal_NormalData();
            normals.isTextured = material.normalsTexture != null && material.normalsTexture.Id != ulong.MaxValue;
            if (normals.isTextured)
                normals.id = material.normalsTexture.Id;
            else
                normals.normalsColor = material.normalsColor;

            roughness = new Internal_RoughnessData();
            roughness.isTextured = material.roughnessTexture != null && material.roughnessTexture.Id != ulong.MaxValue;
            if (roughness.isTextured)
                roughness.id = material.roughnessTexture.Id;
            else
                roughness.roughnessValue = material.roughnessValue;

            metallic = new Internal_MetallicData();
            metallic.isTextured = material.metallicTexture != null && material.metallicTexture.Id != ulong.MaxValue;
            if (metallic.isTextured)
                metallic.id = material.metallicTexture.Id;
            else
                metallic.metallicValue = material.metallicValue;
        }

        public void DebugOutput()
        {
            Console.WriteLine($"Diffuse isTextured: {diffuse.isTextured}");
            if (diffuse.isTextured)
                Console.WriteLine($"Diffuse texture id: {diffuse.id}");
            else
                Console.WriteLine($"Diffuse color: {diffuse.diffuseColor}");
            
            Console.WriteLine($"Normals isTextured: {normals.isTextured}");
            if (normals.isTextured)
                Console.WriteLine($"Normals texture id: {normals.id}");
            else
                Console.WriteLine($"Normals color: {normals.normalsColor}");
            
            Console.WriteLine($"Roughness isTextured: {roughness.isTextured}");
            if (roughness.isTextured)
                Console.WriteLine($"Diffuse texture id: {roughness.id}");
            else
                Console.WriteLine($"Diffuse color: {roughness.roughnessValue}");
            
            Console.WriteLine($"Metallic isTextured: {metallic.isTextured}");
            if (metallic.isTextured)
                Console.WriteLine($"Metallic texture id: {metallic.id}");
            else
                Console.WriteLine($"Metallic color: {metallic.metallicValue}");
        }
    }
    
    [StructLayout(LayoutKind.Explicit)] 
    public struct Internal_DiffuseData
    {
        [FieldOffset(0)] public bool isTextured;
        [FieldOffset(4)] public ulong id;
        [FieldOffset(4)] public Color4 diffuseColor;
    }

    [StructLayout(LayoutKind.Explicit)] 
    public struct Internal_NormalData
    {
        [FieldOffset(0)] public bool isTextured;
        [FieldOffset(4)] public ulong id;
        [FieldOffset(4)] public Color3 normalsColor;
    }
    
    [StructLayout(LayoutKind.Explicit)] 
    public struct Internal_RoughnessData
    {
        [FieldOffset(0)] public bool isTextured;
        [FieldOffset(4)] public ulong id;
        [FieldOffset(4)] public float roughnessValue;
    }
    
    [StructLayout(LayoutKind.Explicit)] 
    public struct Internal_MetallicData
    {
        [FieldOffset(0)] public bool isTextured;
        [FieldOffset(4)] public ulong id;
        [FieldOffset(4)] public float metallicValue;
    }
}
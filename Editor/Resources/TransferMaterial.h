#pragma once
#include "TransformUtils.h"
#include "Utils/OpaqueModelDrawData.h"

struct TransferMaterial
{
    TransferMaterial() = default;

    TransferMaterial(MaterialData data)
    {
        if(data.diffuseData.isTextured)
            diffuse_texture_id = data.diffuseData.textureId;
        else
            diffuse_color = data.diffuseData.color.ToFloat4();

        if(data.normalData.isTextured)
            normals_texture_id = data.normalData.textureId;
        else
            normals_color = data.normalData.normal;

        if(data.roughnessData.isTextured)
            roughness_texture_id = data.roughnessData.textureId;
        else
            roughness_value = data.roughnessData.roughness;

        if(data.metallicData.isTextured)
            metallic_texture_id = data.metallicData.textureId;
        else
            metallic_value = data.metallicData.metallic;
    }
    
    float4 diffuse_color;
    size_t diffuse_texture_id;

    float3 normals_color;
    size_t normals_texture_id;

    float roughness_value;
    size_t roughness_texture_id;

    float metallic_value;
    size_t metallic_texture_id;

    MaterialData Convert()
    {
        MaterialData data = MaterialData();

        data.diffuseData.color = TextureData::Color(diffuse_color.w * 255.0f, diffuse_color.x * 255.0f, diffuse_color.y * 255.0f, diffuse_color.z * 255.0f);
        data.normalData.normal = normals_color;
        data.roughnessData.roughness = roughness_value;
        data.metallicData.metallic = metallic_value;

        if (diffuse_texture_id > 0)
        {
            data.diffuseData.isTextured = true;
            data.diffuseData.textureId = diffuse_texture_id;
        }

        if (normals_texture_id > 0)
        {
            data.normalData.isTextured = true;
            data.normalData.textureId = normals_texture_id;
        }

        if (roughness_texture_id > 0)
        {
            data.roughnessData.isTextured = true;
            data.roughnessData.textureId = roughness_texture_id;
        }

        if (metallic_texture_id > 0)
        {
            data.metallicData.isTextured = true;
            data.metallicData.textureId = metallic_texture_id;
        }

        return data;
    }
};

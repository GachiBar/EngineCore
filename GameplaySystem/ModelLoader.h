#pragma once

#include <RenderEngine.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace engine {

class ModelLoader {
public:
	static bool LoadObj(const std::string& path, ModelMesh& model) {
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(
			path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_FlipWindingOrder);

		if (scene == nullptr) {
			return false;
		}
		if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) {
			return false;
		}

		model.vertices.clear();
		model.indexes.clear();
		model.pt = EPrimitiveType::PRIMITIVETYPE_TRIANGLELIST;

		ProcessNode(model, scene, scene->mRootNode);

		model.primitiveCount = model.indexes.size() / 3;
		return true;
	}

private:
	static void ProcessNode(ModelMesh& model, const aiScene* scene, const aiNode* node) {
		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			ProcessMesh(model, scene, mesh);
		}

		for (size_t i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(model, scene, node->mChildren[i]);
		}
	}

	static void ProcessMesh(ModelMesh& model, const aiScene* scene, const aiMesh* mesh)
	{
		using namespace DirectX::SimpleMath;

		for (size_t i = 0; i < mesh->mNumVertices; ++i) {
			ModelVertex vertex;
			vertex.position = ToVector3(mesh->mVertices[i]);

			if (mesh->HasNormals()) {
				vertex.normal = ToVector3(mesh->mNormals[i]);
			}

			if (mesh->mTextureCoords[0] != nullptr) {
				vertex.texcoord = ToVector2(mesh->mTextureCoords[0][i]);
			}
			else {
				vertex.texcoord = float2{ 0.0f, 0.0f };
			}

			model.vertices.push_back(vertex);
		}

		for (size_t i = 0; i < mesh->mNumFaces; ++i) {
			const aiFace face = mesh->mFaces[i];

			for (size_t j = 0; j < face.mNumIndices; ++j) {
				model.indexes.push_back(face.mIndices[j]);
			}
		}
	}

	static DirectX::SimpleMath::Vector2 ToVector2(aiVector3D vector) {
		return { vector.x, vector.y };
	}

	static DirectX::SimpleMath::Vector3 ToVector3(aiVector3D vector) {
		return { vector.x, vector.y, vector.z };
	}
};

} // namespace engine


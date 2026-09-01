#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

namespace Myztic{
	class Model {
	public:
		Model(const char* path, Scene* linkedScene, Camera* mainCamera, std::vector<Shader>& shaders);

		void pushToRenderer();
	private:
		std::vector<Mesh*> meshes;
		std::string directory;
		Scene* linkedScene;

		std::vector<Texture> loadedTex;

		Camera* modelCamera;

		void processNode(aiNode* node, const aiScene* scene, std::vector<Shader>& shaders);
		Mesh* processMesh(aiMesh* mesh, const aiScene* scene, std::vector<Shader>& shaders);
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};
}

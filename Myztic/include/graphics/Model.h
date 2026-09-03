#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <events/EventDispatcher.h>
#include <events\DrawableEvents.h>

namespace Myztic{
	class Model {
	public:
		Model(const char* path, Scene* linkedScene, Camera* mainCamera, std::vector<Shader>& shaders);

		void pushToRenderer();

		void set_position(glm::vec3& pos) {
			position = pos;
			printf("set position\n");
			EventDispatcher::dispatchBasedOnID<PositionChangeEvent>(EventType::EVENT_POSITION_CHANGE, PositionChangeEvent(linkedScene, pos), modelID);
		}
		glm::vec3& get_position() {
			return position;
		}
		void set_size(glm::vec3& sized) {
			size = sized;
			EventDispatcher::dispatchBasedOnID<ScaleChangeEvent>(EventType::EVENT_SCALE_CHANGE, ScaleChangeEvent(linkedScene, sized), modelID);
		}
		glm::vec3& get_size() {
			return size;
		}
	private:
		std::vector<Mesh*> meshes;
		std::string directory;
		Scene* linkedScene;

		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 size = glm::vec3(1.f, 1.f, 1.f);

		std::vector<Texture> loadedTex;

		uint32_t modelID = 0;
		static uint32_t modelIDIncrement;

		Camera* modelCamera;

		void processNode(aiNode* node, const aiScene* scene, std::vector<Shader>& shaders);
		Mesh* processMesh(aiMesh* mesh, const aiScene* scene, std::vector<Shader>& shaders);
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};
}

#include "pch.h"
#include <display/Window.h>
#include <Scene.h>
#include <graphics\Mesh.h>
#include <graphics\Model.h>
#include <graphics/backend/Texture2D.hpp>
#include <graphics\TexturedDrawable.h>
#include <graphics\primitives\Cube.h>

using namespace Myztic;
Cube* cub;
uint32_t Model::modelIDIncrement = 0;

Model::Model(const char* modelPath, Scene* linkedScene, Camera* mainCamera, std::vector<Shader>& shaders) : linkedScene(linkedScene), modelCamera(mainCamera) {
	Assimp::Importer imp;
	const aiScene* scene = imp.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		const char* errStr = imp.GetErrorString();
		printf("ASSIMP ERROR, COULD NOT LOAD MODEL FROM PATH: %s BECAUSE: %s", modelPath, errStr);
		return;
	}

	this->modelID = modelIDIncrement++;

	//starts the recursion process
	processNode(scene->mRootNode, scene, shaders);
	cub = Cube::makeCube(linkedScene, minVtx, maxVtx, 1.f, glm::vec4(1.f, 1.f, 1.f, 0.2f), {PrecompiledShaders::color_vs, PrecompiledShaders::color_fs});
	cub->camera = mainCamera;
}

void Model::pushToRenderer() {
	for (Mesh* mesh : meshes) {
		this->linkedScene->loadedWin->renderer.drawables.push_back(mesh);
	}
	linkedScene->loadedWin->renderer.drawables.push_back(cub);
}

void Model::processNode(aiNode* node, const aiScene* scene, std::vector<Shader>& shaders) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene, shaders));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene, shaders);
	}
}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene, std::vector<Shader>& shaders) {
	VertexLayout layout = VertexLayout{}.Append(VertexLayout::Position3D);
	
	if (mesh->mNormals) {
		//todo: implement later
		//layout = layout.Append(VertexLayout::Position3D);
	}

	if (mesh->mColors) {
		//todo: implement later
		//layout = layout.Append(VertexLayout::Float4Color);
	}

	VertexBuffer vbuf = VertexBuffer(layout.Append(VertexLayout::Texture2D));
	std::vector<::Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		//figure this out tomorrow
		::Vertex vertex;
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		if (mesh->mTextureCoords[0])
			vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		minVtx.x = std::min<float>(minVtx.x, vertex.Position.x);
		minVtx.y = std::min<float>(minVtx.y, vertex.Position.y);
		minVtx.z = std::min<float>(minVtx.z, vertex.Position.z);

		maxVtx.x = std::max<float>(maxVtx.x, vertex.Position.x);
		maxVtx.y = std::max<float>(maxVtx.y, vertex.Position.y);
		maxVtx.z = std::max<float>(maxVtx.z, vertex.Position.z);

		vertices.push_back(vertex);
	}

	if (mesh->mNormals)
	{

	}

	if (mesh->mColors) {

	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuse = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuse.begin(), diffuse.end());

		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	for (::Vertex vertex : vertices)
		vbuf.EmplaceBack(vertex.Position, vertex.TexCoords);

	return new Mesh(linkedScene, vbuf, indices, textures, this->modelCamera, shaders, this->modelID);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	std::vector<Texture> textures;
	
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < loadedTex.size(); j++) {
			if (std::strcmp(loadedTex[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(loadedTex[j]);
				skip = true;
				break;
			}
		}

		if (!skip) {
			Texture texture;
			texture.type = typeName;
			texture.path = str.C_Str();
			texture.texture = Texture2D::fromFile(directory + str.C_Str());
		
			textures.push_back(texture);
			loadedTex.push_back(texture);
		}
	}

	return textures;
}

#include "pch.h"
#include <graphics\Drawable.h>
#include <graphics/PrecompiledShaders.h>
#include <graphics/backend/Texture2D.hpp>
#include <graphics/Mesh.h>

using namespace Myztic;

Mesh::Mesh(Scene* linkedScene, VertexBuffer& vbuf, std::vector<GLuint> indices, std::vector<Texture>& textures, Camera* camera, std::vector<Shader>& shaders, uint32_t id) : Drawable(linkedScene, vbuf, id)
{
	this->textures = textures;
	this->vert_indices = indices;

	this->vert_type = GL_TRIANGLES;
	vbo.bind();

	vbo.fill((void*)vbuf.GetData(), vbuf.SizeBytes(), GL_STATIC_DRAW);

	inputLayout.setVertexLayout();
	inputLayout.enableAllAttribs();

	if (!(shaders.size() > 0)) {
		shaders.push_back(PrecompiledShaders::texture_vs);
		shaders.push_back(PrecompiledShaders::texture_fs);
	}

	//i hate doing this fucking loop
	for (Shader shader : shaders)
		shaderProgram.attach(shader);

	shaderProgram.link();

	inputLayout.unbind();
	vbo.unbind();
	shaderProgram.unbind();

	this->useEBO(this->vert_indices);

	this->camera = camera;
}

void Mesh::prepareDraw() {
	Drawable::prepareDraw();

	unsigned int diffuseNum = 1;
	unsigned int specularNum = 1;
	for (unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].type;

		if (name == "texture_diffuse")
			number = std::to_string(diffuseNum++);
		else if (name == "texture_specular")
			number = std::to_string(specularNum++);

		this->shaderProgram.uniform1i((name + number), i);
		
		textures[i].texture.bind();
	}
}

void Mesh::finishDraw() {
	Drawable::finishDraw();
	glActiveTexture(GL_TEXTURE0);
}
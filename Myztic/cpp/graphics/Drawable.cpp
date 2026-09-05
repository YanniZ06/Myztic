#include "pch.h"

#include <graphics\Drawable.h>
#include <glm\gtc\type_ptr.hpp>
#include <Scene.h>
#include <display\Window.h>
#include <graphics\Camera.h>
#include <events\EventDispatcher.h>
#include <events\DrawableEvents.h>

using namespace Myztic;

Drawable::Drawable(Scene* linkedScene, std::vector<InputProperty>& inputProperties, uint32_t id) : linkedScene(linkedScene), id(id) {
	this->inputLayout = ShaderInputLayout(ShaderInputLayout::createLayoutDescription(inputProperties));
	vertexData = VertexBuffer(std::move(VertexLayout{}.Append(VertexLayout::Position3D)));
	this->vbo = VBO::make();
	this->linkedScene = linkedScene;

	EventDispatcher::registerEvent<PositionChangeEvent>(EVENT_POSITION_CHANGE, std::function<void(PositionChangeEvent)>([&](PositionChangeEvent pce) {
		if (pce.renderingScene == this->linkedScene) {
			set_position(pce.newPosition);
		}
	}), id);
	EventDispatcher::registerEvent<ScaleChangeEvent>(EVENT_SCALE_CHANGE, std::function<void(ScaleChangeEvent)>([&](ScaleChangeEvent sce) {
		if (sce.renderingScene == this->linkedScene) {
			set_size(sce.newScale);
		}
	}), id);
	EventDispatcher::registerEvent<RotationChangeEvent>(EVENT_ROTATION_CHANGE, std::function<void(RotationChangeEvent)>([&](RotationChangeEvent rce) {
		if (rce.renderingScene == this->linkedScene) {
			set_rotation(rce.newRotation);
		}
	}), id);
}

Drawable::Drawable(Scene* linkedScene, VertexBuffer& vertData, uint32_t id) : linkedScene(linkedScene), id(id) {
	std::vector<InputProperty> ips = vertData.GetLayout().GetDescription().inputProperties;
	this->inputLayout = ShaderInputLayout(ShaderInputLayout::createLayoutDescription(ips));
	//copy operation, safe to get rid of the original variable.
	vertexData = vertData;
	this->vbo = VBO::make();
	this->linkedScene = linkedScene;

	EventDispatcher::registerEvent<PositionChangeEvent>(EVENT_POSITION_CHANGE, std::function<void(PositionChangeEvent)>([&](PositionChangeEvent pce) {
		if (pce.renderingScene == this->linkedScene) {
			set_position(pce.newPosition);
		}
	}), id);
	EventDispatcher::registerEvent<ScaleChangeEvent>(EVENT_SCALE_CHANGE, std::function<void(ScaleChangeEvent)>([&](ScaleChangeEvent sce) {
		if (sce.renderingScene == this->linkedScene) {
			set_size(sce.newScale);
		}
	}), id);
	EventDispatcher::registerEvent<RotationChangeEvent>(EVENT_ROTATION_CHANGE, std::function<void(RotationChangeEvent)>([&](RotationChangeEvent rce) {
		if (rce.renderingScene == this->linkedScene) {
			set_rotation(rce.newRotation);
		}
	}), id);
}

Drawable::~Drawable() {
	linkedScene = nullptr;
}

/*Drawable Drawable::makeQuad(Scene* linkedScene, VertexBuffer& verts, std::vector<Shader> shaders) {
	std::vector<InputProperty> ips = verts.GetLayout().GetDescription().inputProperties;
	Drawable obj = Drawable(linkedScene, verts);
	obj.vert_type = GL_TRIANGLES;
	obj.vbo.bind();
	//reconsider this especially for GL_STATIC_DRAW.
	obj.vbo.fill((void*)verts.GetData(), verts.SizeBytes(), GL_STATIC_DRAW);

	obj.inputLayout.setVertexLayout();
	obj.inputLayout.enableAllAttribs();

	obj.useEBO();

	obj.vert_indices = std::vector<GLuint>({ 0, 1, 3, 1, 2, 3 });

	obj.ebo.bind();
	obj.ebo.fill(obj.vert_indices.data(), sizeof(GLuint) * static_cast<GLint>(obj.vert_indices.size()), GL_STATIC_DRAW);

	for (Shader shader : shaders) 
		obj.shaderProgram.attach(shader);

	obj.shaderProgram.link();

	obj.inputLayout.unbind();
	obj.vbo.unbind();

	return obj;
}*/

// prepare draw
void Drawable::prepareDraw() {
	shaderProgram.bind();

	glUniformMatrix4fv(shaderProgram.getUniformLocation("world"), 1, GL_FALSE, glm::value_ptr(transformation));
	glUniformMatrix4fv(shaderProgram.getUniformLocation("view"), 1, GL_FALSE, (camera == nullptr) ? glm::value_ptr(glm::mat4(1.f)) : glm::value_ptr(camera->intern_ViewMatrix));
	glUniformMatrix4fv(shaderProgram.getUniformLocation("projection"), 1, GL_FALSE, (camera == nullptr) ? glm::value_ptr(glm::mat4(1.f)) : glm::value_ptr(camera->intern_ProjecMatrix));
	inputLayout.bindInputLayout();
}

void Drawable::finishDraw() {
	shaderProgram.unbind();
	inputLayout.unbind();
}
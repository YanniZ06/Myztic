#pragma once
#include <string>
#include <vector>
#include <graphics\backend\Texture2D.hpp>
#include <graphics\Drawable.h>

struct Vertex {
	glm::vec3 Position;
	//we can include normals later, for now i want vertex positions and texcoords.
	//glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	Myztic::Texture2D texture;
	std::string type;
	std::string path;
};

namespace Myztic {
	class Mesh : public Myztic::Drawable {
	public:
		std::vector<Texture> textures;

		Mesh(Myztic::Scene* linkedScene, VertexBuffer& vbuf, std::vector<GLuint> indices, std::vector<Texture>& textures, Camera* camera, std::vector<Shader>& shaders, uint32_t id);
		void prepareDraw() override;
		void finishDraw() override;
	};
}
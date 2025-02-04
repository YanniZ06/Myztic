#pragma once

#include <ErrorHandler.hpp>
#include <map>
#include <iostream>

namespace Myztic {
	class Shader;
	class ShaderProgram {
	public:
		ShaderProgram();
		~ShaderProgram() {

		};
		/**
		 * Links the shader program to the OpenGL pipeline.
		 *
		 */
		inline void link() {
			CHECK_GL(glLinkProgram(handle));
			int success;
			char infoLog[512];
			glGetProgramiv(handle, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(handle, 512, NULL, infoLog);
				std::cout << infoLog << "\n";
			}
		};

		/**
		 * Attaches a shader to the shader program.
		 *
		 * \param shader The shader to attach.
		 */
		void attach(Shader& shader); // Inlined

		/**
		 * Binds the program to the OpenGL pipeline.
		 *
		 */
		inline void bind() {
			CHECK_GL(glUseProgram(this->handle));
		};

		/**
		 * Unbinds the program from the OpenGL pipeline.
		 *
		 */
		static inline void unbind() {
			CHECK_GL(glUseProgram(0));
		};

		/**
		 * Destroys the shader program.
		 *
		 */
		inline void destroy() {
			CHECK_GL(glDeleteProgram(handle));
		};

		/**
		 * Retrieve the uniform's location in the shader.
		 *
		 * \param uniformName name of the uniform
		 * \return The index (location) of the uniform.
		 */
		int getUniformLocation(std::string uniformName);
		/**
		 * Cache the uniform in the uniforms map for faster retrieval.
		 *
		 * \param uniformName The name of the uniform to cache.
		 */
		void cacheUniform(std::string uniformName);

		GLuint handle;
		std::map<std::string, int> uniforms;
	};
}

#include <graphics/backend/Shader.h>
inline void Myztic::ShaderProgram::attach(Myztic::Shader& shader) {
	CHECK_GL(glAttachShader(this->handle, shader.handle));
};
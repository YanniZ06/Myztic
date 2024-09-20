#pragma once

#include <ErrorHandler.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <cpp/stb_image.h>

class Texture2D {
public:
	inline Texture2D(GLuint handle) {
		this->handle = handle;
	}
	inline Texture2D() = default;

	/*
	* Creates a Texture2D instance from fileName 
	* \param fileName Texture file path and name (fileName must include path, file name and extension ex: "assets/images/Sky.png")
	*/
	static Texture2D fromFile(std::string fileName) {
		int width, height, numChannels;
		std::string location = "Assets/Textures/" + fileName;
		unsigned char* fillingData = stbi_load(location.c_str(), &width, &height, &numChannels, 0);

		Texture2D ret = Texture2D::make();
		ret.bind();
		ret.configureTexture();

		//if clear data returned
		if (fillingData) {
			ret.fill(width, height, fillingData);
		}
		else printf("Could not load texture of filename: %s", fileName.c_str());
		stbi_image_free(fillingData);

		return ret;
	};

	/*
	* Creates a singular Texture2D.
	* 
	*/
	static inline Texture2D make() {
		Texture2D ret;
		CHECK_GL(glGenTextures(1, &ret.handle));
		return ret;
	};

	template <int AMOUNT>
	/*
	* Creates multiple instances of Texture2D  
	* 
	*/
	inline static std::array<Texture2D, AMOUNT> makeNum() {
		//this uses the default constructor too !!!
		std::array<Texture2D, AMOUNT> rets;

		for (int i = 0; i < AMOUNT; i++)
			CHECK_GL(glGenTextures(1, &rets[i].vao));

		return rets;
	};

	/* 
	* Binds the texture to the OpenGL pipeline.  
	* 
	*/
	inline void bind() {
		CHECK_GL(glBindTexture(GL_TEXTURE_2D, handle));
	};

	/*
	* Unbinds the texture from the OpenGL pipeline.
	* 
	*/
	inline static void unbind() {
		CHECK_GL(glBindTexture(GL_TEXTURE_2D, 0));
	};

	/*
	* Destroys the texture and calls the deconstructor. 
	* 
	*/
	inline void destroy() {
		CHECK_GL(glDeleteTextures(1, &handle));
	};

	/*
	 * Configures the Texture2D settings, by default it's antialiasing off and mipmaps on, mirrored repeat. 
	 * 
	 */
	inline void configureTexture() {
		CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
		CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));

		//yanni said this will be for antialiasing off (mipmaps are enabled on this)
		CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
		CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	};

	/* 
	* Fills the Texture2D with the image file's data.
	*   
	*/
	inline void fill(int width, int height, unsigned char* data) {
		CHECK_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)data));
		CHECK_GL(glGenerateMipmap(GL_TEXTURE_2D));
	};
private:
	GLuint handle;
};
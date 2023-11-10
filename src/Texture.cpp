#include "Texture.h"

#include "Common.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char* path, bool useMipMaps)
	: Texture()
{
	LoadFromFile(path, useMipMaps);
}

Texture::~Texture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &m_texture);
}

bool Texture::LoadFromFile(const char *path, bool useMipMaps)
{
	int width, height, numChannels;
	unsigned char *data = stbi_load(path, &width, &height, &numChannels, 0);
	if (data == nullptr)
	{
		printf("Error loading texture %s: %s\n", path, stbi_failure_reason());
		ASSERT(false);
	}

	m_width = width;
	m_height = height;

	// create and bind texture
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// set the texture data
	GLint format;
	switch(numChannels)
	{
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			ASSERT(false && "Unsupported number of channels");
	}
	glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

	// set params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (useMipMaps)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	// unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

Texture* Texture::CreateUtilTexture()
{
	Texture* utilTexture = new Texture();

	unsigned char data[] = { 255, 255, 255 }; 
	utilTexture->m_width = 1;
	utilTexture->m_height = 1;

	// create and bind texture
	glGenTextures(1, &utilTexture->m_texture);
	glBindTexture(GL_TEXTURE_2D, utilTexture->m_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, utilTexture->m_width, utilTexture->m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	// set params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	return utilTexture;
}

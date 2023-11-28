#include "Texture.h"

#include "Common.h"

// todo: an stb wrapper
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::~Texture()
{
	if (m_id != 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &m_id);
	}
}

bool Texture::Load(const std::string& path)
{
	stbi_set_flip_vertically_on_load(true);

	int width, height, numChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
	if (data == nullptr)
	{
		printf("Error loading texture %s: %s\n", path.c_str(), stbi_failure_reason());
		ASSERT(false);
	}

	m_width = width;
	m_height = height;

	// create and bind texture
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	// set the texture data
	GLint format = 0;
	switch (numChannels)
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool Texture::Load(const std::string& path, bool useMipMaps)
{
	stbi_set_flip_vertically_on_load(true);

	int width, height, numChannels;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
	if (data == nullptr)
	{
		printf("Error loading texture %s: %s\n", path.c_str(), stbi_failure_reason());
		ASSERT(false);
	}

	m_width = width;
	m_height = height;

	// create and bind texture
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	// set the texture data
	GLint format = 0;
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_id);
}

Texture* Texture::CreateUtilTexture()
{
	Texture* utilTexture = new Texture();

	unsigned char data[] = { 255, 255, 255 }; 
	utilTexture->m_width = 1;
	utilTexture->m_height = 1;

	// create and bind texture
	glGenTextures(1, &utilTexture->m_id);
	glBindTexture(GL_TEXTURE_2D, utilTexture->m_id);

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

void Texture::GenerateMipMaps()
{
	glBindTexture(GL_TEXTURE_2D, m_id);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);
}

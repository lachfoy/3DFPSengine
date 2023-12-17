#include "Common.h"
#include "Texture.h"

#include "STBImage.h"

Texture::~Texture()
{
	if (m_id != 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &m_id);
	}
}

std::shared_ptr<Texture> Texture::Create(const std::string& path)
{
	std::shared_ptr<Texture> texture = std::make_shared<Texture>();

	int width, height, numChannels;
	unsigned char* data = STBImage::Load(path.c_str(), &width, &height, &numChannels, true);
	if (data == nullptr)
	{
		printf("Error loading texture %s\n", path.c_str());
		ASSERT(false);
		return nullptr;
	}

	texture->m_width = width;
	texture->m_height = height;

	// create and bind texture
	glGenTextures(1, &texture->m_id);
	glBindTexture(GL_TEXTURE_2D, texture->m_id);

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
	{
		ASSERT(false && "Unsupported number of channels");
		return nullptr;
	}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, texture->m_width, texture->m_height, 0, format, GL_UNSIGNED_BYTE, data);
	delete data;

	// set params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

std::shared_ptr<Texture> Texture::CreateUtilTexture()
{
	std::shared_ptr<Texture> utilTexture = std::make_shared<Texture>();

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

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::GenerateMipMaps()
{
	glBindTexture(GL_TEXTURE_2D, m_id);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);
}

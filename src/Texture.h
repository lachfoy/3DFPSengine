#pragma once

#include <glad/glad.h>

class Texture
{
public:
	Texture() = default;
	Texture(const char* path, bool useMipMaps = false);
	~Texture();

	bool LoadFromFile(const char* path, bool useMipMaps = false);
	void Bind() const;

	static Texture* CreateUtilTexture();

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

private:
	GLuint m_texture;
	int m_width;
	int m_height;

};

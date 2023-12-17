#pragma once

#include <glad/glad.h>
#include <string>

#include <memory>

class Texture
{
public:
	Texture() {}
	~Texture();

	static std::shared_ptr<Texture> Create(const std::string& path);
	static std::shared_ptr<Texture> CreateUtilTexture();

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	void Bind() const;
	void GenerateMipMaps();

private:
	GLuint m_id = 0;
	int m_width = 0;
	int m_height = 0;

};

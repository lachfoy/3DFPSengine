#pragma once

#include "iResource.h"
#include <glad/glad.h>
#include <string>

class Texture : public iResource
{
public:
	Texture() = default;
	~Texture();

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	static Texture* Create();

	bool Load(const std::string& path);

	bool Load(const std::string& path, bool useMipMaps = false);

	void Bind() const;

	static Texture* CreateUtilTexture();

	void GenerateMipMaps();

private:
	GLuint m_id;
	int m_width;
	int m_height;

};

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include <vector>

#include <string>

#include "UIVertex.h"

class Font;
class TextRenderer
{
public:
	TextRenderer() = default;
	~TextRenderer() {}

	void Init();
	void SetProjection(unsigned int screenWidth, unsigned int screenHeight);
	void Dispose();

	void AddStringToBatch(std::string string, float x, float y, glm::vec3 color, float alpha = 1.0f);

	void RenderQuads();

private:
	void FlushQuads();

	void CreateShaderProgram();
	void CreateRenderData();

	GLuint m_shaderProgram;

	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;

	std::vector<UIVertex> m_textVertices;
	std::vector<unsigned int> m_textIndices;

	GLint m_colorUniformLocation;
	GLint m_textureUniformLocation;

	Font* m_font;

};

extern TextRenderer gTextRenderer;

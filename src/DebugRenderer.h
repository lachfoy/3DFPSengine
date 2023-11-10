#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Texture.h"

#include <vector>
#include <map>

struct DebugLine {
	glm::vec2 p1;
	glm::vec2 p2;
	glm::vec3 color;
	float duration;
};

class DebugRenderer
{
public:
	DebugRenderer() = default;
	~DebugRenderer() {}

	void Init();
	void SetProjection(unsigned int screenWidth, unsigned int screenHeight);
	void Dispose();

	void AddLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec3& color, float duration = -1.0f);
	void AddCircle(const glm::vec2& center, float radius, const glm::vec3& color, float duration = -1.0f);
	void Update(float dt);
	void Render();

private:
	void CreateShaderProgram();
	void CreateRenderData();

	void CheckError();

	// Debug lines
	GLuint m_debugShaderProgram;
	std::vector<DebugLine> m_lines;
	GLuint m_lineVao;
	GLuint m_lineVbo;

	GLint m_colorUniformLocation;

};

extern DebugRenderer gDebugRenderer;
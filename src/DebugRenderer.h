#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Texture.h"

#include <vector>
#include <map>

struct DebugVertex
{
	glm::vec3 position;
	glm::vec3 color;
};

struct DebugLine {
	glm::vec3 to;
	glm::vec3 from;
	glm::vec3 color;
	float duration;
};

//struct LineGroup
//{
//	std::vector<DebugLine> lines;
//	float duration;
//};
//
//std::map<int, LineGroup> lineGroups;

class Camera;

class DebugRenderer
{
public:
	DebugRenderer() = default;
	~DebugRenderer();

	void Init();
	void Dispose();

	void AddLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color, float duration = -1.0f);

	void AddSphere(const glm::vec3& center, float radius, const glm::vec3& color, float duration = -1.0f);
	void AddBox(const glm::vec3& center, const glm::vec3& halfExtents, const glm::vec3& color, float duration = -1.0f);

	void Render(Camera* camera);
	void PostRenderUpdate(float dt);

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
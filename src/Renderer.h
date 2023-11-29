#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <list>

class Camera;
class iRenderable;

typedef std::list<iRenderable*> tRenderList;

class Renderer
{
public:
	Renderer() = default;
	~Renderer();

	void Init();

	void AddToRenderList(iRenderable* renderable);
	void Render(Camera* camera);

private:
	void CreateShaderProgram();

	GLuint m_shaderProgram;
	tRenderList m_renderList;
	GLint m_projectionUniformLocation;
	GLint m_viewUniformLocation;
	GLint m_cameraLocalPosUniformLocation;
	GLint m_modelUniformLocation;
	GLint m_colorUniformLocation;

};

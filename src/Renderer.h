#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <list>
#include "NonCopyable.h"

class Camera;
class Renderable;

typedef std::list<Renderable*> tRenderList;

class Renderer : public NonCopyable
{
public:
	Renderer() = default;
	~Renderer();

	void Init();

	void AddToRenderList(Renderable* renderable);
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

extern Renderer gRenderer;

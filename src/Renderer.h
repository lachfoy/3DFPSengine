#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <vector>
#include <list>

class Camera;
class Texture;
class Mesh;
class iRenderable;

typedef std::list<iRenderable*> tRenderList;

class Renderer
{
public:
	Renderer() = default;
	~Renderer();

	void Init();
	void SetProjection(const glm::mat4& projection);

	void AddToRenderList(iRenderable* renderable);
	void Render(Camera* camera);

private:
	void CreateShaderProgram();

	GLuint m_shaderProgram;
	GLuint m_quadVAO;
	tRenderList m_renderList;

};

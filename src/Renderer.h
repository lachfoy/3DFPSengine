#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <vector>
#include <list>

class Camera;
class Texture;
class Mesh;
class IRenderable;

typedef std::list<IRenderable*> tRenderList;

class Renderer
{
public:
	Renderer() = default;
	~Renderer();

	void Init();
	void SetProjection(const glm::mat4& projection);

	void AddToRenderList(IRenderable* renderable);
	void Render(Camera* camera);

	void RenderQuad(glm::vec3 pos, glm::vec3 size, const Texture* texture, glm::vec3 color = glm::vec3(1.0f)); // not used
	void RenderMesh(const Mesh* mesh, const glm::mat4& transform, const Texture* texture, glm::vec3 color = glm::vec3(1.0f)); // not used

private:
	void CreateShaderProgram();

	GLuint m_shaderProgram;


	tRenderList m_renderList;

	GLuint m_quadVAO;


};

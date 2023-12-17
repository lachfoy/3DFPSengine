#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "Texture.h"
#include "Mesh.h"
#include <memory>
#include <vector>

class Camera;

struct RenderObject
{
	std::shared_ptr<Mesh> mesh;
	glm::mat4 transform;
	std::shared_ptr<Texture> texture;
};

typedef std::vector<RenderObject> tRenderList;

class Renderer
{
public:
	Renderer() = default;
	~Renderer();

	void Init();

	void AddToRenderList(std::shared_ptr<Mesh>& mesh, glm::mat4 transform, std::shared_ptr<Texture>& texture);

	void Blit(std::shared_ptr<Texture> texture, glm::mat4 transformation = glm::mat4(1.0f));

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

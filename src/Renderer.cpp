#include "Renderer.h"

#include <glm/gtc/type_ptr.hpp>

#include "Common.h"
#include "Camera.h"

#include "Renderable.h"

Renderer gRenderer;

Renderer::~Renderer()
{
	if (m_shaderProgram != 0)
	{
		glDeleteProgram(m_shaderProgram);
	}
}

void Renderer::Init()
{
	CreateShaderProgram();

	glUseProgram(m_shaderProgram);
	m_projectionUniformLocation = glGetUniformLocation(m_shaderProgram, "u_projection");
	m_viewUniformLocation = glGetUniformLocation(m_shaderProgram, "u_view");
	m_cameraLocalPosUniformLocation = glGetUniformLocation(m_shaderProgram, "u_cameraLocalPos");
	m_modelUniformLocation = glGetUniformLocation(m_shaderProgram, "u_model");
	m_colorUniformLocation = glGetUniformLocation(m_shaderProgram, "u_color");

	// Set up sampler ...just one for now
	GLint textureUniformLocation = glGetUniformLocation(m_shaderProgram, "u_sampler");
	assert(textureUniformLocation >= 0 && "Sampler does not exist");
	glUniform1i(textureUniformLocation, 0);
	glUseProgram(0);
}

void Renderer::AddToRenderList(std::shared_ptr<Mesh>& mesh, glm::mat4 transform, std::shared_ptr<Texture>& texture)
{
	m_renderList.emplace_back(RenderObject{ mesh, transform, texture });
}

void Renderer::Blit(std::shared_ptr<Texture> texture, glm::mat4 transformation)
{

}

void Renderer::Render(Camera* camera)
{
	glUseProgram(m_shaderProgram);

	glUniformMatrix4fv(m_projectionUniformLocation, 1, false, glm::value_ptr(camera->GetProjection()));
	glUniformMatrix4fv(m_viewUniformLocation, 1, false, glm::value_ptr(camera->GetView()));
	glUniform3fv(m_cameraLocalPosUniformLocation, 1, glm::value_ptr(camera->m_position));

	for (const auto& renderObject : m_renderList)
	{
		// set transformation
		glUniformMatrix4fv(m_modelUniformLocation, 1, false, glm::value_ptr(renderObject.transform));

		// bind texture 1
		glActiveTexture(GL_TEXTURE0);
		renderObject.texture->Bind();

		// set color
		glUniform4fv(m_colorUniformLocation, 1, glm::value_ptr(glm::vec4(1.0f))); // Use a material

		// draw
		renderObject.mesh->Bind();
		glDrawElements(GL_TRIANGLES, (GLsizei)renderObject.mesh->IndexCount(), GL_UNSIGNED_INT, 0);
	}

	// GL CHECK ERROR

	// RenderDebug()
	m_renderList.clear();
}

void Renderer::CreateShaderProgram()
{
	// Create the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	{
		const GLchar* vertexSource = R"(
			#version 330 core

			layout (location = 0) in vec3 a_position;
			layout (location = 1) in vec3 a_normal;
			layout (location = 2) in vec2 a_texcoord;

			out vec3 v_fragPos;
			out vec3 v_normal;
			out vec2 v_texcoord;

			uniform mat4 u_projection;
			uniform mat4 u_view;
			uniform mat4 u_model;

			void main()
			{
				gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
				v_fragPos = vec3(u_model * vec4(a_position, 1.0));
				v_normal = transpose(inverse(mat3(u_model))) * a_normal;
				v_texcoord = a_texcoord;
			}
		)";

		glShaderSource(vertexShader, 1, &vertexSource, 0);
		glCompileShader(vertexShader);

		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			printf("Failed to compile vertex shader:\n%s\n", infoLog);
		}
	}

	// Create the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	{
		const GLchar* fragmentSource = R"(
			#version 330 core
			out vec4 out_color;

			in vec3 v_fragPos;
			in vec3 v_normal;
			in vec2 v_texcoord;

			uniform vec4 u_color;
			uniform sampler2D u_sampler;

			uniform vec3 u_cameraLocalPos;

			vec3 lightDir = vec3(0.5, 1.0, 0.5);
			vec3 ambient = vec3(0.3, 0.3, 0.3);

			void main()
			{
				vec3 norm = normalize(v_normal);
				float diff = max(dot(norm, lightDir), 0.0);

				vec3 finalColor = texture(u_sampler, v_texcoord).rgb * (diff + ambient);

				out_color = vec4(finalColor, 1.0);
			}
		)";

		glShaderSource(fragmentShader, 1, &fragmentSource, 0);
		glCompileShader(fragmentShader);

		int success;
		char infoLog[512];
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			printf("Failed to compile fragment shader:\n%s\n", infoLog);
		}
	}

	// Link the final shader program
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertexShader);
	glAttachShader(m_shaderProgram, fragmentShader);
	glLinkProgram(m_shaderProgram);

	int success;
	char info_log[512];
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, info_log);
		printf("Failed to link shader:\n%s\n", info_log);
	}

	// No longer need these
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

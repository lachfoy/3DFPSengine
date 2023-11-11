#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Common.h"
#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"
#include "IRenderable.h"

Renderer::~Renderer()
{
	glDeleteProgram(m_shaderProgram);
}

void Renderer::Init()
{
    CreateShaderProgram();
}

void Renderer::SetProjection(int screenWidth, int screenHeight)
{
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 100.0f);

    glUseProgram(m_shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "u_projection"), 1, false, glm::value_ptr(projection));

}

void Renderer::AddToRenderList(IRenderable *renderable)	
{	
    m_renderList.push_back(renderable);	
}

void Renderer::Render(Camera* camera)
{
    // render normal scene
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram(m_shaderProgram);

	// todo cache uniform locations
	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "u_view"), 1, false, glm::value_ptr(camera->GetView()));
	glUniform3fv(glGetUniformLocation(m_shaderProgram, "u_cameraLocalPos"), 1, glm::value_ptr(camera->m_position));

    for (const auto& renderable : m_renderList)
    {
        // set transformation
		glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "u_transform"), 1, false, glm::value_ptr(renderable->m_transform));

        // bind texture 1
        glActiveTexture(GL_TEXTURE0);
        renderable->m_texture->Bind();

        // set color
		glUniform4fv(glGetUniformLocation(m_shaderProgram, "u_color"), 1, glm::value_ptr(glm::vec4(1.0f)));


        // draw
        renderable->m_mesh->Draw();

        // unbind texture 1
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0); // unbind texture
    }

    glUseProgram(0);
}

void Renderer::CreateShaderProgram()
{
	// Create the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	{
		const GLchar* vertexSource = R"(
			#version 330 core

			layout (location = 0) in vec2 a_position;
			layout (location = 1) in vec2 a_texcoord;

			varying vec2 v_texcoord;

			uniform mat4 u_projection;

			void main()
			{
				gl_Position = u_projection * vec4(a_position, 0.0, 1.0);
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

			in vec2 v_texcoord;

			uniform vec4 u_color;
			uniform sampler2D u_sampler;

			void main()
			{
				out_color = texture(u_sampler, v_texcoord);
				//out_color = vec4(1, 0, 1, 1);
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

	// Set up sampler ...just one for now
	glUseProgram(m_shaderProgram);
	GLint textureUniformLocation = glGetUniformLocation(m_shaderProgram, "u_sampler");
	assert(textureUniformLocation >= 0 && "Sampler does not exist");
	glUniform1i(textureUniformLocation, 0);
}

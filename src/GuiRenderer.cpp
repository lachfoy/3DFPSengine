#include "GuiRenderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cassert>
#include <algorithm>

static const unsigned int kMaxQuads = 1000;

void GuiRenderer::Init()
{
	CreateShaderProgram();
	CreateRenderData();

	m_utilTexture = Texture::CreateUtilTexture();
}

void GuiRenderer::SetProjection(unsigned int screenWidth, unsigned int screenHeight)
{
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 0.0f, -1.0f, 1.0f);
	
	glUseProgram(m_shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "u_projection"), 1, false, glm::value_ptr(projection));
}

void GuiRenderer::Dispose()
{
	delete m_utilTexture;

	glDeleteBuffers(1, &m_ebo);
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);

	glDeleteProgram(m_shaderProgram);
}

void GuiRenderer::AddQuadToBatch(float x, float y, float w, float h, glm::vec3 color, float alpha)
{
	AddTexturedQuadToBatch(x, y, w, h, m_utilTexture, color, alpha);
}

void GuiRenderer::AddTexturedQuadToBatch(float x, float y, float w, float h, Texture* texture, glm::vec3 color, float alpha)
{
	QuadEntry quadEntry;
	quadEntry.vertices[0] = UIVertex::Make(x, y + h, 0.0f, 1.0f);
	quadEntry.vertices[1] = UIVertex::Make(x + w, y + h, 1.0f, 1.0f);
	quadEntry.vertices[2] = UIVertex::Make(x, y, 0.0f, 0.0f);
	quadEntry.vertices[3] = UIVertex::Make(x + w, y, 1.0f, 0.0f);
	quadEntry.color = color;
	quadEntry.alpha = alpha;
	quadEntry.texture = texture;

	m_quadEntries.push_back(quadEntry);
}

void GuiRenderer::RenderQuads()
{
	glUseProgram(m_shaderProgram);
	glBindVertexArray(m_vao);

	Texture* currentTexture = nullptr;
	glm::vec3 currentColor = glm::vec3(1.0f);
	float currentAlpha = 1.0f;

	for (const QuadEntry& obj : m_quadEntries)
	{
		if (obj.texture != currentTexture || obj.color != currentColor || obj.alpha != currentAlpha)
		{
			FlushQuads();

			// Update the current state
			currentTexture = obj.texture;
			currentColor = obj.color;
			currentAlpha = obj.alpha;

			// Bind the new texture
			currentTexture->Bind();

			// Update the color and alpha
			glUniform4fv(m_colorUniformLocation, 1, glm::value_ptr(glm::vec4(currentColor, currentAlpha)));
		}

		// Add vertices and indices to the batch
		const UIVertex* vertices = obj.vertices;
		const unsigned int vertexOffset = m_quadVertices.size();
		for (int i = 0; i < 4; i++)
		{
			m_quadVertices.push_back(vertices[i]);
		}

		m_quadIndices.push_back(vertexOffset);
		m_quadIndices.push_back(vertexOffset + 1);
		m_quadIndices.push_back(vertexOffset + 2);
		m_quadIndices.push_back(vertexOffset + 1);
		m_quadIndices.push_back(vertexOffset + 3);
		m_quadIndices.push_back(vertexOffset + 2);
	}

	FlushQuads();

	glBindVertexArray(0);
	m_quadEntries.clear();
}

void GuiRenderer::FlushQuads()
{
	if (!m_quadVertices.empty())
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_quadVertices.size() * sizeof(UIVertex), m_quadVertices.data());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_quadIndices.size() * sizeof(unsigned int), m_quadIndices.data());

		glDrawElements(GL_TRIANGLES, m_quadIndices.size(), GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		m_quadVertices.clear();
		m_quadIndices.clear();
	}
}

void GuiRenderer::CreateShaderProgram()
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
			uniform sampler2D u_texture;

			void main()
			{
				out_color = texture(u_texture, v_texcoord) * u_color;
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

	m_colorUniformLocation = glGetUniformLocation(m_shaderProgram, "u_color");
	m_textureUniformLocation = glGetUniformLocation(m_shaderProgram, "u_texture");

	// Tell the shader which texture unit to use
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(m_textureUniformLocation, 0);

	// No longer need these
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

void GuiRenderer::CreateRenderData()
{
	// VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// VBO
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, kMaxQuads * 4 * sizeof(UIVertex), NULL, GL_DYNAMIC_DRAW);

	// EBO
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, kMaxQuads * 6 * sizeof(unsigned int), NULL, GL_DYNAMIC_DRAW);

	// Enable the vertex attribute arrays for position and texcoords
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)(sizeof(float)*2));

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

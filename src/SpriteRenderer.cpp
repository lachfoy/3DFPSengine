#include "SpriteRenderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cassert>
#include <algorithm>

static const unsigned int kMaxQuads = 2000;

void SpriteRenderer::Init()
{
	CreateShaderProgram();
	CreateRenderData();
}

void SpriteRenderer::SetProjection(unsigned int screenWidth, unsigned int screenHeight)
{
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 0.0f, -1.0f, 1.0f);
	
	glUseProgram(m_shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "u_projection"), 1, false, glm::value_ptr(projection));
}

void SpriteRenderer::Dispose()
{
	glDeleteBuffers(1, &m_ebo);
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);

	glDeleteProgram(m_shaderProgram);
}

void SpriteRenderer::AddRenderObject(const RenderObject& renderObject)
{
	m_renderObjects.push_back(renderObject);
}

void SpriteRenderer::RenderObjects()
{
	//std::sort(m_renderObjects.begin(), m_renderObjects.end(), RenderObjectCompare());

	glUseProgram(m_shaderProgram);
	glBindVertexArray(m_vao);

	Texture* currentTexture = nullptr;

	for (const RenderObject& obj : m_renderObjects)
	{
		if (obj.GetTexture() != currentTexture)
		{
			// If a different texture is encountered, start a new batch
			if (!m_vertexBuffer.empty())
			{
				currentTexture->Bind();
				FlushBatch();
				ClearBatch();
			}
			currentTexture = obj.GetTexture();
		}


		// Create a scale matrix for horizontal flip
		glm::mat3 flipMatrix = glm::mat3(
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		);

		for (Vertex vertex : *(obj.GetVertexVec()))
		{
			if (obj.GetFlipPolicy() == FlipPolicy::FlipX)
			{
				vertex.position = glm::vec2(glm::vec3(vertex.position, 1.0f) * flipMatrix);
			}

			float rotation = obj.GetRotation();
			float x = vertex.position.x;
			vertex.position.x = x * cos(rotation) - vertex.position.y * sin(rotation);
			vertex.position.y = x * sin(rotation) + vertex.position.y * cos(rotation);

			if (obj.GetPosition() != nullptr)
				vertex.position += *(obj.GetPosition());

			m_vertexBuffer.push_back(vertex);
		}

		// Append the indices from the current RenderObject to the current index batch
		unsigned int vertexOffset = m_vertexBuffer.size() - obj.GetVertexVec()->size();
		for (unsigned int index : *(obj.GetIndexVec()))
		{
			m_indexBuffer.push_back(index + vertexOffset);
		}
	}

	// Add the last batch (if any) to the result
	if (!m_vertexBuffer.empty())
	{
		currentTexture->Bind();
		FlushBatch();
		ClearBatch();
	}

	glBindVertexArray(0);
	m_renderObjects.clear();
}

void SpriteRenderer::FlushBatch()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertexBuffer.size() * sizeof(Vertex), m_vertexBuffer.data());
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_indexBuffer.size() * sizeof(unsigned int), m_indexBuffer.data());
	
	glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteRenderer::ClearBatch()
{
	m_vertexBuffer.clear();
	m_indexBuffer.clear();
}

void SpriteRenderer::CreateShaderProgram()
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

void SpriteRenderer::CreateRenderData()
{
	// VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// VBO
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, kMaxQuads * 4 * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);

	// EBO
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, kMaxQuads * 6 * sizeof(unsigned int), NULL, GL_DYNAMIC_DRAW);

	// Enable the vertex attribute arrays for position and texcoords
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SpriteRenderer::CheckError()
{
	GLenum error = glGetError();
	if (error > 0) {
		switch (error)
		{
		case GL_INVALID_ENUM:
			printf("GL_INVALID_ENUM");
			break;
		case GL_INVALID_OPERATION:
			printf("GL_INVALID_OPERATION");
			break;
		case GL_INVALID_VALUE:
			printf("GL_INVALID_VALUE");
			break;
		default:
			break;
		}
		//assert(false);
	}
}

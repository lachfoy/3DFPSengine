#include "DebugRenderer.h"

#include <glm/gtc/type_ptr.hpp>
#include "Common.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <algorithm>

DebugRenderer gDebugRenderer;

static const unsigned int kMaxLines = 1000;
static const unsigned int kCircleSegments = 32;

void DebugRenderer::Init()
{
	CreateShaderProgram();
	CreateRenderData();
}

void DebugRenderer::SetProjection(unsigned int screenWidth, unsigned int screenHeight)
{
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 0.0f, -1.0f, 1.0f);

	glUseProgram(m_debugShaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(m_debugShaderProgram, "u_projection"), 1, false, glm::value_ptr(projection));
}

void DebugRenderer::Dispose()
{
	glDeleteBuffers(1, &m_lineVbo);
	glDeleteVertexArrays(1, &m_lineVao);

	glDeleteProgram(m_debugShaderProgram);
}

void DebugRenderer::AddLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec3& color, float duration)
{
	DebugLine line;
	line.p1 = p1;
	line.p2 = p2;
	line.color = color;
	line.duration = duration;
	m_lines.push_back(line);
}

void DebugRenderer::AddCircle(const glm::vec2& center, float radius, const glm::vec3& color, float duration)
{
	float angleIncrement = 2.0f * PI / kCircleSegments;

	for (int i = 0; i < kCircleSegments; i++)
	{
		float angle1 = i * angleIncrement;
		float angle2 = (i + 1) * angleIncrement;

		glm::vec2 p1 = center + glm::vec2(radius * std::cos(angle1), radius * std::sin(angle1));
		glm::vec2 p2 = center + glm::vec2(radius * std::cos(angle2), radius * std::sin(angle2));

		DebugLine line;
		line.p1 = p1;
		line.p2 = p2;
		line.color = color;
		line.duration = duration;
		m_lines.push_back(line);
	}
}

void DebugRenderer::Update(float dt)
{
	auto it = m_lines.begin();
	while (it != m_lines.end())
	{
		if (it->duration == -1.0f) // skip if the duration is -1
		{
			++it;
		}
		else
		{
			it->duration -= dt;
			if (it->duration <= 0.0f)
			{
				it = m_lines.erase(it);
			}
			else
			{
				++it;
			}
		}
		
	}
}

void DebugRenderer::Render()
{
	glUseProgram(m_debugShaderProgram);
	glBindVertexArray(m_lineVao);

	glBindBuffer(GL_ARRAY_BUFFER, m_lineVbo);

	std::vector<glm::vec2> lineData;
	for (const auto& debugLine : m_lines)
	{
		lineData.push_back(debugLine.p1);
		lineData.push_back(debugLine.p2);
	}
	glBufferSubData(GL_ARRAY_BUFFER, 0, lineData.size() * sizeof(glm::vec2), lineData.data());

	for (size_t i = 0; i < m_lines.size(); ++i) {
		// Set the color uniform for the current line
		glUniform3fv(m_colorUniformLocation, 1, glm::value_ptr(m_lines[i].color));

		// Render the line
		glDrawArrays(GL_LINES, i * 2, 2);
	}

	//glDrawArrays(GL_LINES, 0, m_lines.size() * 2);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void DebugRenderer::CreateShaderProgram()
{
	// Create the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	{
		const GLchar* vertexSource = R"(
		#version 330 core

		layout (location = 0) in vec2 a_position;

		uniform mat4 u_projection;

		void main()
		{
			gl_Position = u_projection * vec4(a_position, 0.0, 1.0);
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

		uniform vec3 u_color;

		void main()
		{
			out_color = vec4(u_color, 1);
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
	m_debugShaderProgram = glCreateProgram();
	glAttachShader(m_debugShaderProgram, vertexShader);
	glAttachShader(m_debugShaderProgram, fragmentShader);
	glLinkProgram(m_debugShaderProgram);

	int success;
	char info_log[512];
	glGetProgramiv(m_debugShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_debugShaderProgram, 512, NULL, info_log);
		printf("Failed to link shader:\n%s\n", info_log);
	}

	m_colorUniformLocation = glGetUniformLocation(m_debugShaderProgram, "u_color");
	
	// No longer need these
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void DebugRenderer::CreateRenderData()
{
	// FOR DEBUG LINES
	// VAO
	glGenVertexArrays(1, &m_lineVao);
	glBindVertexArray(m_lineVao);

	// VBO
	glGenBuffers(1, &m_lineVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVbo);
	glBufferData(GL_ARRAY_BUFFER, kMaxLines * sizeof(glm::vec2) * 2, NULL, GL_DYNAMIC_DRAW);

	// Enable the vertex attribute arrays for position and texcoords
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DebugRenderer::CheckError()
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

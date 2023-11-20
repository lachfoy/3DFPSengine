#include "DebugRenderer.h"

#include <glm/gtc/type_ptr.hpp>
#include "Common.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <algorithm>

#include "Camera.h"

DebugRenderer gDebugRenderer;

static const unsigned int kMaxLines = 10000;

void DebugRenderer::Init()
{
	CreateShaderProgram();
	CreateRenderData();
}

void DebugRenderer::SetProjection(const glm::mat4& projection)
{
	glUseProgram(m_debugShaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(m_debugShaderProgram, "u_projection"), 1, false, glm::value_ptr(projection));
}

void DebugRenderer::Dispose()
{
	glDeleteBuffers(1, &m_lineVbo);
	glDeleteVertexArrays(1, &m_lineVao);

	glDeleteProgram(m_debugShaderProgram);
}

void DebugRenderer::AddLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color, float duration)
{
	DebugLine line;
	line.from = from;
	line.to = to;
	line.color = color;
	line.duration = duration;
	m_lines.push_back(line);
}

void DebugRenderer::AddSphere(const glm::vec3& center, float radius, const glm::vec3& color, float duration)
{
	int latitudeLines = 8;//  # Number of lines along the latitude.Increase for higher detail.
	int longitudeLines = 8;//  # Number of lines along the longitude.Increase for higher detail.

	for (int i = 0; i < latitudeLines; i++)
	{
		float lat1 = PI * (-0.5 + float(i) / latitudeLines);
		float lat2 = PI * (-0.5 + float(i + 1) / latitudeLines);

		for (int j = 0; j < longitudeLines; j++)
		{
			float long1 = 2 * PI * float(j) / longitudeLines;
			float long2 = 2 * PI * float(j + 1) / longitudeLines;

			//# Calculate the four vertices of the current segment
			glm::vec3 vertex1 = center + radius * glm::vec3(cos(lat1) * cos(long1), cos(lat1) * sin(long1), sin(lat1));
			glm::vec3 vertex2 = center + radius * glm::vec3(cos(lat2) * cos(long1), cos(lat2) * sin(long1), sin(lat2));
			glm::vec3 vertex3 = center + radius * glm::vec3(cos(lat1) * cos(long2), cos(lat1) * sin(long2), sin(lat1));
			glm::vec3 vertex4 = center + radius * glm::vec3(cos(lat2) * cos(long2), cos(lat2) * sin(long2), sin(lat2));

			//# Draw lines between these vertices
			AddLine(vertex1, vertex2, color, duration);
			AddLine(vertex1, vertex3, color, duration);
			AddLine(vertex2, vertex4, color, duration);
			AddLine(vertex3, vertex4, color, duration);
		}
	}
}

void DebugRenderer::Render(Camera* camera)
{
	glUseProgram(m_debugShaderProgram);
	glBindVertexArray(m_lineVao);

	glBindBuffer(GL_ARRAY_BUFFER, m_lineVbo);

	glUniformMatrix4fv(glGetUniformLocation(m_debugShaderProgram, "u_view"), 1, false, glm::value_ptr(camera->GetView()));

	std::vector<glm::vec3> lineData;
	for (const auto& debugLine : m_lines)
	{
		lineData.push_back(debugLine.from);
		lineData.push_back(debugLine.to);
	}
	glBufferSubData(GL_ARRAY_BUFFER, 0, lineData.size() * sizeof(glm::vec3), lineData.data());

	for (size_t i = 0; i < m_lines.size(); ++i) {
		// Set the color uniform for the current line
		glUniform3fv(m_colorUniformLocation, 1, glm::value_ptr(m_lines[i].color));

		// Render the line
		// todo.... batch these
		glDrawArrays(GL_LINES, i * 2, 2);
	}

	//glDrawArrays(GL_LINES, 0, m_lines.size() * 2);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void DebugRenderer::PostRenderUpdate(float dt)
{
	auto it = m_lines.begin();
	while (it != m_lines.end())
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

void DebugRenderer::CreateShaderProgram()
{
	// Create the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	{
		const GLchar* vertexSource = R"(
		#version 330 core

		layout (location = 0) in vec3 a_position;

		uniform mat4 u_projection;
		uniform mat4 u_view;

		void main()
		{
			gl_Position = u_projection * u_view * vec4(a_position, 1.0);
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
	glBufferData(GL_ARRAY_BUFFER, kMaxLines * sizeof(glm::vec3) * 2, NULL, GL_DYNAMIC_DRAW);

	// Enable the vertex attribute arrays for position and texcoords
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

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

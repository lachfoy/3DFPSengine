#include "DebugRenderer.h"

#include <glm/gtc/type_ptr.hpp>
#include "Common.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <algorithm>

#include "Camera.h"

DebugRenderer gDebugRenderer;

static const unsigned int kMaxLines = 10000;

DebugRenderer::~DebugRenderer()
{
	glDeleteBuffers(1, &m_lineVbo);
	glDeleteVertexArrays(1, &m_lineVao);

	if (m_debugShaderProgram != 0)
	{
		glDeleteProgram(m_debugShaderProgram);
	}
}

void DebugRenderer::Init()
{
	CreateShaderProgram();
	CreateRenderData();
}

void DebugRenderer::AddLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color, float duration)
{
	if (m_lines.size() != kMaxLines)
	{
		m_lines.push_back({ to, from, color, duration });
	}
	else
	{
		printf("Warning: Debug renderer max lines has been reached\n");
	}
}

void DebugRenderer::AddSphere(const glm::vec3& center, float radius, const glm::vec3& color, float duration)
{
	ASSERT(false && "Don't use this its broken for some reason");

	int latitudeLines = 8;//  # Number of lines along the latitude.Increase for higher detail.
	int longitudeLines = 8;//  # Number of lines along the longitude.Increase for higher detail.

	for (int i = 0; i < latitudeLines; i++)
	{
		float lat1 = PI * (-0.5f + float(i) / latitudeLines);
		float lat2 = PI * (-0.5f + float(i + 1) / latitudeLines);

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

void DebugRenderer::AddBox(const glm::vec3& center, const glm::vec3& halfExtents, const glm::vec3& color, float duration)
{
	// Calculate the vertices of the box
	glm::vec3 vertices[8];
	for (int i = 0; i < 8; ++i)
	{
		vertices[i].x = center.x + halfExtents.x * ((i & 1) ? 1 : -1);
		vertices[i].y = center.y + halfExtents.y * ((i & 2) ? 1 : -1);
		vertices[i].z = center.z + halfExtents.z * ((i & 4) ? 1 : -1);
	}

	// Lines along the x-axis
	AddLine(vertices[0], vertices[1], color, duration);
	AddLine(vertices[2], vertices[3], color, duration);
	AddLine(vertices[4], vertices[5], color, duration);
	AddLine(vertices[6], vertices[7], color, duration);

	// Lines along the y-axis
	AddLine(vertices[0], vertices[2], color, duration);
	AddLine(vertices[1], vertices[3], color, duration);
	AddLine(vertices[4], vertices[6], color, duration);
	AddLine(vertices[5], vertices[7], color, duration);

	// Lines along the z-axis
	AddLine(vertices[0], vertices[4], color, duration);
	AddLine(vertices[1], vertices[5], color, duration);
	AddLine(vertices[2], vertices[6], color, duration);
	AddLine(vertices[3], vertices[7], color, duration);
}

void DebugRenderer::Render(Camera* camera)
{
	glUseProgram(m_debugShaderProgram);
	glBindVertexArray(m_lineVao);

	glUniformMatrix4fv(glGetUniformLocation(m_debugShaderProgram, "u_projection"), 1, false, glm::value_ptr(camera->m_projection));
	glUniformMatrix4fv(glGetUniformLocation(m_debugShaderProgram, "u_view"), 1, false, glm::value_ptr(camera->GetView()));

	glBindBuffer(GL_ARRAY_BUFFER, m_lineVbo);

	std::vector<DebugVertex> vertices;
	for (const auto& line : m_lines)
	{
		vertices.push_back({ line.from, line.color });
		vertices.push_back({ line.to, line.color });
	}
	
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(DebugVertex), vertices.data());
	glDrawArrays(GL_LINES, 0, (GLsizei)vertices.size());

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
			layout (location = 1) in vec3 a_color;

			uniform mat4 u_projection;
			uniform mat4 u_view;

			out vec3 v_color;

			void main()
			{
				v_color = a_color;
				gl_Position = u_projection * u_view * vec4(a_position, 1.0);
			}
		)";

		glShaderSource(vertexShader, 1, &vertexSource, 0);
		glCompileShader(vertexShader);

		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			printf("Failed to compile vertex shader:\n%s\n", infoLog);
		}
	}

	// Create the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	{
		const GLchar* fragmentSource = R"(
			#version 330 core

			in vec3 v_color;
			out vec4 out_color;

			void main()
			{
				out_color = vec4(v_color, 1);
			}
		)";

		glShaderSource(fragmentShader, 1, &fragmentSource, 0);
		glCompileShader(fragmentShader);

		int success;
		char infoLog[512];
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
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
	char infoLog[512];
	glGetProgramiv(m_debugShaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_debugShaderProgram, 512, NULL, infoLog);
		printf("Failed to link shader:\n%s\n", infoLog);
	}

	// No longer need these
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void DebugRenderer::CreateRenderData()
{
	// VAO
	glGenVertexArrays(1, &m_lineVao);
	glBindVertexArray(m_lineVao);

	// VBO
	glGenBuffers(1, &m_lineVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVbo);
	glBufferData(GL_ARRAY_BUFFER, kMaxLines * sizeof(DebugVertex) * 2, nullptr, GL_DYNAMIC_DRAW);

	// Enable the vertex attribute arrays for position and color
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color));

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

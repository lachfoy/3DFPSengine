#include "TextRenderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cassert>
#include <algorithm>
#include "Font.h"

TextRenderer gTextRenderer;

static const unsigned int kMaxQuads = 1000;

void TextRenderer::Init()
{
	CreateShaderProgram();
	CreateRenderData();

	m_font = new Font();
	m_font->Load("data/fonts/smallfonts_10px.fnt");
}

void TextRenderer::SetProjection(unsigned int screenWidth, unsigned int screenHeight)
{
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 0.0f, -1.0f, 1.0f);
	
	glUseProgram(m_shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "u_projection"), 1, false, glm::value_ptr(projection));
}

void TextRenderer::Dispose()
{
	delete m_font;

	glDeleteBuffers(1, &m_ebo);
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);

	glDeleteProgram(m_shaderProgram);
}

void TextRenderer::AddStringToBatch(std::string string, float x, float y, glm::vec3 color, float alpha)
{
	// Initialize lastChar with an invalid character ID
	unsigned int lastChar = UINT_MAX;  // Maximum value means no last character

	// iterate through all characters
	for (char c : string)
	{
		CharInfo charInfo = m_font->GetInfo(c);

		float textureW = static_cast<float>(m_font->m_texture->GetWidth());
		float textureH = static_cast<float>(m_font->m_texture->GetHeight());

		// If we had a previous character, look up the kerning pair and adjust x
		if (lastChar != UINT_MAX) {
			short kerningAmount = m_font->GetKerningAmount(lastChar, charInfo.id);
			x += kerningAmount;
		}

		// Calculate the texture coordinates
		float tx = charInfo.x / textureW;
		float ty = charInfo.y / textureH;
		float tw = charInfo.width / textureW;
		float th = charInfo.height / textureH;

		// Calculate positions
		float posx = x + charInfo.xoffset;
		float posy = y + charInfo.yoffset;
		float w = charInfo.width;
		float h = charInfo.height;

		// todo actually have a batch of text render data so I can change fonts/color

		// Define the vertex offset for indexing
		const unsigned int vertexOffset = m_textVertices.size();

		m_textVertices.push_back(UIVertex::Make(posx, posy, tx, ty));// Lower left vertex
		m_textVertices.push_back(UIVertex::Make(posx + w, posy, tx + tw, ty));// Lower right vertex
		m_textVertices.push_back(UIVertex::Make(posx, posy + h, tx, ty + th));// Upper left vertex
		m_textVertices.push_back(UIVertex::Make(posx + w, posy + h, tx + tw, ty + th));// Upper right vertex

		m_textIndices.push_back(vertexOffset);
		m_textIndices.push_back(vertexOffset + 1);
		m_textIndices.push_back(vertexOffset + 2);
		m_textIndices.push_back(vertexOffset + 1);
		m_textIndices.push_back(vertexOffset + 3);
		m_textIndices.push_back(vertexOffset + 2);

		x += charInfo.xadvance;

		lastChar = charInfo.id;
	}
}

void TextRenderer::RenderQuads()
{
	glUseProgram(m_shaderProgram);
	glBindVertexArray(m_vao);

	// Bind the new texture
	m_font->m_texture->Bind();

	// Update the color and alpha
	glUniform4fv(m_colorUniformLocation, 1, glm::value_ptr(glm::vec4(1.0f)));

	FlushQuads();

	glBindVertexArray(0);
}

void TextRenderer::FlushQuads()
{
	if (!m_textVertices.empty())
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_textVertices.size() * sizeof(UIVertex), m_textVertices.data());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_textIndices.size() * sizeof(unsigned int), m_textIndices.data());

		glDrawElements(GL_TRIANGLES, m_textIndices.size(), GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		m_textVertices.clear();
		m_textIndices.clear();
	}
}

void TextRenderer::CreateShaderProgram()
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

void TextRenderer::CreateRenderData()
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

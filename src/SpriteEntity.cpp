#include "SpriteEntity.h"

#include "Common.h"
#include "DebugRenderer.h"
#include "Texture.h"

#include <glm/gtc/matrix_transform.hpp>

SpriteEntity::SpriteEntity(glm::vec2 position, glm::vec2 size, Texture* texture)
	: m_size(size), m_texture(texture)
{
	m_position = position;

	// Create the mesh data
	// This should be handled by a different class though
	Vertex vertices[] = {
		{ glm::vec2(-m_size.x / 2.0f, -m_size.y / 2.0f), glm::vec2(0.0f, 0.0f) }, // Bottom-left corner
		{ glm::vec2(m_size.x / 2.0f, -m_size.y / 2.0f), glm::vec2(1.0f, 0.0f) },  // Bottom-right corner
		{ glm::vec2(-m_size.x / 2.0f, m_size.y / 2.0f), glm::vec2(0.0f, 1.0f) },  // Top-left corner
		{ glm::vec2(m_size.x / 2.0f, m_size.y / 2.0f), glm::vec2(1.0f, 1.0f) }    // Top-right corner
	};

	for (const auto& vertex : vertices) {
		m_vertexVec.push_back(vertex);
	}

	unsigned int indices[] = {
		0, 1, 2,
		1, 3, 2
	};

	for (const auto& index : indices) {
		m_indexVec.push_back(index);
	}

	// Set the radius acording to the largest dimension of size
	m_radius = m_size.x >= m_size.y ? m_size.x * 0.5f : m_size.y * 0.5f;
}

void SpriteEntity::SetRotation(float radians)
{
	m_rotation = radians;
}

void SpriteEntity::SetFlipPolicy(FlipPolicy flipPolicy)
{
	m_flipPolicy = flipPolicy;
}

void SpriteEntity::Render(SpriteRenderer* renderer)
{
	ASSERT(m_texture && "No texture was set!");

	RenderObject renderObject = RenderObject(
		&m_vertexVec,
		&m_indexVec,
		&m_position,
		m_texture,
		m_rotation,
		m_flipPolicy
	);

	renderer->AddRenderObject(std::move(renderObject));
}

void SpriteEntity::RenderDebugQuad()
{
	// Calculate a bounding rectangle
	glm::vec2 min{0.0f};
	glm::vec2 max{0.0f};

	for (const auto& vertex : m_vertexVec)
	{
		if (vertex.position.x < min.x) min.x = vertex.position.x;
		if (vertex.position.y < min.y) min.y = vertex.position.y;
		if (vertex.position.x > max.x) max.x = vertex.position.x;
		if (vertex.position.y > max.y) max.y = vertex.position.y;
	}

	//glm::vec2 topL = m_position - (m_size * 0.5f);
	//glm::vec2 bottomR = m_position + (m_size * 0.5f);
	//glm::vec2 bottomL = glm::vec2(topL.x, bottomR.y);
	//glm::vec2 topR = glm::vec2(bottomR.x, topL.y);

	glm::vec2 topL = min + m_position;
	glm::vec2 bottomR = max + m_position;
	glm::vec2 bottomL = glm::vec2(topL.x, bottomR.y);
	glm::vec2 topR = glm::vec2(bottomR.x, topL.y);

	const glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);
	gDebugRenderer.AddLine(topL, bottomL, color);
	gDebugRenderer.AddLine(bottomL, bottomR, color);
	gDebugRenderer.AddLine(bottomR, topR, color);
	gDebugRenderer.AddLine(topR, topL, color);
}

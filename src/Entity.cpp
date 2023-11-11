#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>

Entity::Entity() : m_worldPosition(glm::vec3(0.0f, 0.0f, 0.0f)), m_scale(glm::vec3(1.0f, 1.0f, 1.0f)), m_rotation(glm::vec3(0.0f, 0.0f, 0.0f))
{
	UpdateTransform();
}

Entity::Entity(const glm::vec3& worldPosition, const glm::vec3& scale, const glm::vec3& rotation)
	: m_worldPosition(worldPosition), m_scale(scale), m_rotation(rotation)
{
	UpdateTransform();
}

void Entity::UpdateTransform()
{
	glm::mat4 transform = glm::mat4(1.0f); // identity

	transform = glm::scale(transform, m_scale);

	transform = glm::rotate(transform, m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	transform = glm::translate(transform, m_worldPosition);

	m_transform = transform;
}

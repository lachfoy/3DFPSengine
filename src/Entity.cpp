#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>

Entity::Entity()
{
	m_position = glm::vec2(0.0f, 0.0f);
}

void Entity::SetPosition(const glm::vec2& position)
{
	m_position = position;
}

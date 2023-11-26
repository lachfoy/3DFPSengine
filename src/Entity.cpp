#include "Entity.h"

Entity::Entity() : m_position(glm::vec3(0.0f, 0.0f, 0.0f))
{
}

Entity::Entity(const glm::vec3& position)
	: m_position(position)
{
}

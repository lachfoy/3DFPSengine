#include "Entity.h"

int Entity::s_idCounter{ 0 };

Entity::Entity() : m_position(glm::vec3(0.0f, 0.0f, 0.0f))
{
	m_id = s_idCounter++;

	printf("Added entity with id:%d\n", m_id);
}

Entity::Entity(const glm::vec3& position)
	: m_position(position)
{
	m_id = s_idCounter++;

	printf("Added entity with id:%d\n", m_id);
}

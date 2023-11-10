#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Entity
{
public:
	Entity();
	~Entity() {}

	glm::vec2 GetPosition() const { return m_position; }
	void SetPosition(const glm::vec2& position);

	void Remove() { m_remove = true; }
	bool GetRemove() const { return m_remove; }

	int m_index = 0;

protected:
	glm::vec2 m_position;
	bool m_remove = false;

};
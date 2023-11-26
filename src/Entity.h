#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Entity
{
public:
	Entity();
	Entity(const glm::vec3& position);
	~Entity() {}

	virtual void Create() {};
	virtual void FixedUpdate() {};
	virtual void Update(float dt) {};
	virtual void Destroy() {};

protected:
	glm::vec3 m_position;

};
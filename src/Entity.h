#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Entity
{
public:
	Entity();
	Entity(const glm::vec3& worldPosition, const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f), const glm::vec3& rotation = glm::vec3(0.0f, 0.0f, 0.0f));
	~Entity() {}

	virtual void OnCreate() {};
	virtual void OnUpdate(float dt) {};
	virtual void OnDestroy() {};

protected:
	virtual void UpdateTransform();

	glm::vec3 m_worldPosition;
	glm::vec3 m_scale;
	glm::vec3 m_rotation;

	glm::mat4 m_transform;

};
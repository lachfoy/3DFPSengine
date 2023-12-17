#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Scene;

// ONLY Created by scene
class Entity
{
friend class Scene;
public:
	Entity();
	Entity(const glm::vec3& position);
	~Entity() {}

	glm::vec3 GetPosition() const { return m_position; }

	virtual void Create() {}
	virtual void FixedUpdate() {}
	virtual void Update(float dt) {}
	virtual void Render() {}
	virtual void Destroy() {}

protected:
	glm::vec3 m_position;
	Scene* m_scene = nullptr;
	int m_id;
	static int s_idCounter;

};
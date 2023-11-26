#pragma once

// interface for 3D render objects
// should contain
// - geometry: vertices, normals etc.
// - transformation matrices
// - material/texture properties

#include "Entity.h"
#include <glm/glm.hpp>

class Mesh;
class Texture;

class iRenderable : public Entity
{
friend class Renderer;
public:
	iRenderable();
	~iRenderable() {}

	//const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f), const glm::vec3& rotation = glm::vec3(0.0f, 0.0f, 0.0f)

	void UpdateTransform();

protected:
	glm::vec3 m_scale;
	glm::vec3 m_rotation;

	glm::mat4 m_transform;

	Mesh* m_mesh = nullptr;
	Texture* m_texture = nullptr;

};

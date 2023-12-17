#pragma once

// interface for 3D render objects
// should contain
// - geometry: vertices, normals etc.
// - transformation matrices
// - material/texture properties

#include "Entity.h"
#include <glm/glm.hpp>

#include <memory>
#include "Mesh.h"
#include "Texture.h"

class Renderable : public Entity
{
friend class Renderer;
public:
	Renderable();
	~Renderable() {}

	//const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f), const glm::vec3& rotation = glm::vec3(0.0f, 0.0f, 0.0f)

	void UpdateTransform();

	void Render() override;

protected:
	glm::vec3 m_scale;
	glm::vec3 m_rotation;

	glm::mat4 m_transform;

	std::shared_ptr<Mesh> m_mesh;
	std::shared_ptr<Texture> m_texture;

};

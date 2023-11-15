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

class IRenderable : public Entity
{
friend class Renderer;
public:
	IRenderable() = default;
	~IRenderable() {}

protected:
	//glm::mat4 m_transform;
	Mesh* m_mesh = nullptr;
	Texture* m_texture = nullptr;

};

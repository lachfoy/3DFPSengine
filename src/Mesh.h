#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

#include <btBulletDynamicsCommon.h>

#include <memory>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

class Mesh
{
public:
	Mesh() = default;
	~Mesh();

	static std::shared_ptr<Mesh> Create(const std::string& filepath);
	static std::shared_ptr<Mesh> CreateQuad(const glm::vec2& size);

	void Bind() const;

	size_t IndexCount() const { return indices.size(); }


	// todo split this out into an ObjLoader helper file
	static btTriangleMesh* LoadCollisionMeshFromOBJ(const std::string& filepath);

private:
	GLuint m_vertexArrayId;
	GLuint m_vertexBufferId;

	GLuint m_elementBufferId;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

};

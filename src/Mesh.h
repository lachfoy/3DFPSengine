#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

#include <btBulletDynamicsCommon.h>

class Mesh
{
public:
	Mesh() = default;
	~Mesh();

	static Mesh* CreateQuad(const glm::vec2& size);

	void Draw() const;

	// todo split this out into an ObjLoader helper file
	static Mesh* LoadMeshFromOBJ(const std::string& filepath);
	static btTriangleMesh* LoadCollisionMeshFromOBJ(const std::string& filepath);

	int NumTriangles() const { return m_numTriangles; }
	
private:
	GLuint m_vertexArrayId;
	GLuint m_vertexBufferId;
	int m_numTriangles;

};

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

#include <bullet/btBulletDynamicsCommon.h>

class Mesh
{
public:
	Mesh() = default;
	~Mesh();

	// todo split this out into an ObjLoader helper file
	static Mesh* CreateMeshFromFile(std::string filepath);
	//static btTriangleMesh* CreateCollisionMeshFromFile(std::string filepath);

	void Draw() const;

	int NumTriangles() const { return m_numTriangles; }
	
private:
	GLuint m_vertexArrayId;
	GLuint m_vertexBufferId;
	int m_numTriangles;

};

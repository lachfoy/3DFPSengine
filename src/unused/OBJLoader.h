#pragma once

class Mesh;
class btTriangleMesh;

#include <string>

class OBJLoader
{
public:
	static Mesh* LoadMeshFromOBJ(const std::string& path);
	static btTriangleMesh* LoadCollisionMeshFromOBJ(const std::string& path);

};

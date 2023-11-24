#pragma once

#include <map>
#include <string>

// CURRENTLY NOT USED

class Mesh;

typedef std::map<std::string, Mesh*> tMeshMap;

class MeshManager
{
public:
	MeshManager() {}
	~MeshManager() {}

	void UnloadResources();
	bool LoadMesh(const std::string& name, const std::string& path);

	const Mesh* GetMesh(const std::string& name) const;

private:
	tMeshMap m_meshMap;

};

extern MeshManager gMeshManager;

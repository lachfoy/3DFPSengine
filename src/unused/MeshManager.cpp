#include "MeshManager.h"

#include "Common.h"
#include <iostream>

#include "Mesh.h"

MeshManager gMeshManager;

void MeshManager::UnloadResources()
{
	std::cout << "Unloading meshes\n";
	std::cout << "--------------------------------------------------------\n";
	for (auto& it : m_meshMap)
	{
		delete it.second;
		std::cout << "Unloaded mesh: " << it.first << "\n";
	}
	std::cout << "--------------------------------------------------------\n\n";
}

bool MeshManager::LoadMesh(const std::string& name, const std::string& path)
{
	Mesh* mesh = Mesh::CreateMeshFromFile(path);
	m_meshMap[name] = mesh;

	return true;
}

const Mesh* MeshManager::GetMesh(const std::string& name) const
{
	const Mesh* temp = m_meshMap.at(name);
	if (!temp)
	{
		ASSERT_MSG(false, "ERROR: missing mesh:%s \n", name.c_str());
	}

	return temp;
}
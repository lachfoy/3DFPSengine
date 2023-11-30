#include "Common.h"
#include "ResourceManager.h"

#include "Texture.h"
#include "Mesh.h"
#include "Sound.h"

void ResourceManager::LoadMesh(const std::string& id, const std::string& path)
{
	auto it = m_meshMap.find(id);
	if (it != m_meshMap.end())
	{
		ASSERT_MSG(false, "Mesh with id \"%s\" already exists", id.c_str());
	}

	//m_meshMap[id] = OBJLoader::LoadMeshFromOBJ(path);
}

Mesh* ResourceManager::GetMesh(const std::string& id)
{
	auto it = m_meshMap.find(id);
	ASSERT_MSG(it != m_meshMap.end(), "Mesh with id \"%s\" doesn't exist", id.c_str());

	return it == m_meshMap.end() ? nullptr : it->second;
}

void ResourceManager::LoadSound(const std::string& id, const std::string& path)
{
	auto it = m_soundMap.find(id);
	if (it != m_soundMap.end())
	{
		ASSERT_MSG(false, "Sound with id \"%s\" already exists", id.c_str());
	}

	//m_soundMap[id] = SoundLoader::LoadSoundFromFile(path);
	m_soundMap[id] = Sound::LoadFromFile(path);
}

Sound* ResourceManager::GetSound(const std::string& id)
{
	auto it = m_soundMap.find(id);
	ASSERT_MSG(it != m_soundMap.end(), "Sound with id \"%s\" doesn't exist", id.c_str());

	return it == m_soundMap.end() ? nullptr : it->second;
}

//void ResourceManager::UnloadResources()
//{
//	printf("Unloading resources...\n");
//	printf("--------------------------------------------------------\n");
//	UnloadTextures();
//	UnloadMeshes();
//	UnloadSounds();
//	printf("--------------------------------------------------------\n\n");
//}

void ResourceManager::UnloadTextures()
{
	for (auto it : m_textureMap)
	{
		SAFE_DELETE(it.second);
		printf("Unloaded texture: %s\n", it.first.c_str());
	}
}

void ResourceManager::UnloadMeshes()
{
	for (auto it : m_meshMap)
	{
		SAFE_DELETE(it.second);
		printf("Unloaded mesh: %s\n", it.first.c_str());
	}
}

void ResourceManager::UnloadSounds()
{
	for (auto it : m_soundMap)
	{
		SAFE_DELETE(it.second);
		printf("Unloaded sound: %s\n", it.first.c_str());
	}
}

void ResourceManager::UnloadResources()
{
	printf("Unloading resources...\n");
	printf("--------------------------------------------------------\n");
	for (auto it : m_resources)
	{
		SAFE_DELETE(it.second);
		printf("Deleted resource: %s\n", it.first.c_str());
	}
	printf("--------------------------------------------------------\n\n");
}

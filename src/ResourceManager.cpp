#include "Common.h"
#include "ResourceManager.h"

class ImageLoader
{
public:
	static Texture* LoadTextureFromFile(const std::string& path) { return nullptr; } // Allocates new texture object
};

class OBJLoader
{
public:
	static Mesh* LoadMeshFromOBJ(const std::string& path) { return nullptr; } // Allocates new texture object
};

class SoundLoader
{
public:
	static Sound* LoadSoundFromFile(const std::string& path) { return nullptr; } // Allocates new texture object
};

void ResourceManager::LoadTexture(const std::string& id, const std::string& path)
{
	auto it = m_textureMap.find(id);
	if (it != m_textureMap.end())
	{
		ASSERT_MSG(false, "Texture with id \"%s\" already exists", id.c_str());
	}

	m_textureMap[id] = ImageLoader::LoadTextureFromFile(path);
}

Texture* ResourceManager::GetTexture(const std::string& id)
{
	auto it = m_textureMap.find(id);
	ASSERT_MSG(it != m_textureMap.end(), "Texture with id \"%s\" doesn't exist", id.c_str());

	return it == m_textureMap.end() ? nullptr : it->second;
}

void ResourceManager::LoadMesh(const std::string& id, const std::string& path)
{
	auto it = m_meshMap.find(id);
	if (it != m_meshMap.end())
	{
		ASSERT_MSG(false, "Mesh with id \"%s\" already exists", id.c_str());
	}

	m_meshMap[id] = OBJLoader::LoadMeshFromOBJ(path);
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

	m_soundMap[id] = SoundLoader::LoadSoundFromFile(path);
}

Sound* ResourceManager::GetSound(const std::string& id)
{
	auto it = m_soundMap.find(id);
	ASSERT_MSG(it != m_soundMap.end(), "Sound with id \"%s\" doesn't exist", id.c_str());

	return it == m_soundMap.end() ? nullptr : it->second;
}

void ResourceManager::UnloadResources()
{
	UnloadTextures();
	UnloadMeshes();
	UnloadSounds();
}

void ResourceManager::UnloadTextures()
{
	printf("Unloading textures...\n");
	printf("--------------------------------------------------------\n");
	for (auto it : m_textureMap)
	{
		SAFE_DELETE(it.second);
		printf("Unloaded texture: %s\n", it.first.c_str());
	}
	printf("--------------------------------------------------------\n\n");
}

void ResourceManager::UnloadMeshes()
{
	printf("Unloading meshes...\n");
	printf("--------------------------------------------------------\n");
	for (auto it : m_meshMap)
	{
		SAFE_DELETE(it.second);
		printf("Unloaded mesh: %s\n", it.first.c_str());
	}
	printf("--------------------------------------------------------\n\n");
}

void ResourceManager::UnloadSounds()
{
	printf("Unloading sounds...\n");
	printf("--------------------------------------------------------\n");
	for (auto it : m_soundMap)
	{
		SAFE_DELETE(it.second);
		printf("Unloaded sound: %s\n", it.first.c_str());
	}
	printf("--------------------------------------------------------\n\n");
}
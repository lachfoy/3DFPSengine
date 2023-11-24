#pragma once

#include <unordered_map>
#include <string>

class Texture;
class Mesh;
class Sound;

class ResourceManager
{
public:
	static ResourceManager& Instance()
	{
		static ResourceManager instance;
		return instance;
	}

	void LoadTexture(const std::string& id, const std::string& path);
	Texture* GetTexture(const std::string& id);

	void LoadMesh(const std::string& id, const std::string& path);
	Mesh* GetMesh(const std::string& id);

	void LoadSound(const std::string& id, const std::string& path);
	Sound* GetSound(const std::string& id);

	void UnloadResources(); // Explicit unloading of resources (rather than in a destructor?) not sure if this is necessary but for now leave it like this

private:
	ResourceManager() {}

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	void UnloadTextures();
	void UnloadMeshes();
	void UnloadSounds();

	std::unordered_map<std::string, Texture*> m_textureMap;
	std::unordered_map<std::string, Mesh*> m_meshMap;
	std::unordered_map<std::string, Sound*> m_soundMap;

};

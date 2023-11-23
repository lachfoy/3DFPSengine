#pragma once

#include <unordered_map>
#include <string>

class Texture;
class Mesh;
class Sound;

class ResourceManager
{
public:
	ResourceManager() {};
	~ResourceManager() = default;

	void LoadTexture(const std::string& id, const std::string& path);
	Texture* GetTexture(const std::string& id);

	void LoadMesh(const std::string& id, const std::string& path);
	Mesh* GetMesh(const std::string& id);

	void LoadSound(const std::string& id, const std::string& path);
	Sound* GetSound(const std::string& id);

	void UnloadResources();

private:
	void UnloadTextures();
	void UnloadMeshes();
	void UnloadSounds();

	std::unordered_map<std::string, Texture*> m_textureMap;
	std::unordered_map<std::string, Mesh*> m_meshMap;
	std::unordered_map<std::string, Sound*> m_soundMap;

};
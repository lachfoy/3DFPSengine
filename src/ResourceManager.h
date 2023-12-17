#pragma once

#include <unordered_map>
#include <string>

class Texture;
class Mesh;
class Sound;

#include <memory>

class ResourceManager
{
public:
	static ResourceManager& Instance()
	{
		static ResourceManager instance;
		return instance;
	}

	void LoadMesh(const std::string& id, const std::string& path);
	Mesh* GetMesh(const std::string& id);

	void LoadSound(const std::string& id, const std::string& path);
	Sound* GetSound(const std::string& id);


	// At the moment, we would expect every single resources to be loaded into memory at the start of the game, and then unloaded when the game quits
	// If we wanted to have resources load per scene, and unload at the end of that scene, we could ref count the resources
	// I.e. when a scene loads we increment the ref count for the resources we need, and then clean up any resources with a ref count of 0.
	// When a scene unloads, we just decrement all of the refs for the resources we were using in that scene 
	// This would require a bit of a redesign of the resource manager though.

	// Also to consider would be "local" vs "global" resources
	// local resources are loaded and unloaded per scene, but global resources would be loaded in memory for the entire game.

	// Could use shared_ptr, but this would mean that "local" resources can not be owned by the resource manager (because otherwise they would just remain in scope for the entire game)
	
	//class ResourceManager
	//{
	//public:
	//	std::shared_ptr<Texture> GetTexture(int id);
	//
	//private:
	//	std::unordered_map<int, std::weak_ptr<Texture>> m_textures;
	//}

	template<typename T>
	void LoadResource(const std::string& path, const std::string& id)
	{
		auto it = m_resources.find(id);
		if (it != m_resources.end())
		{
			// resource already exists
			// error
			printf("Resource already exists\n");
			return;
		}

		// Call factory method that depends on the resource 
		std::shared_ptr<T> resource = T::Create(path);
		if (!resource)
		{
			// error loading the resource
			printf("Error loading resource\n");
			return;
		}

		m_resources[id] = resource;
	}

	template<typename T>
	std::shared_ptr<T> GetResource(const std::string& id)
	{
		auto it = m_resources.find(id);
		if (it != m_resources.end())
		{
			return std::static_pointer_cast<T>(it->second);
		}
		else
		{
			return nullptr; // ERROR
		}
	}

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

	std::unordered_map<std::string, std::shared_ptr<void>> m_resources;

};



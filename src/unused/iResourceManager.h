#pragma once

#include <unordered_map>
#include <string>

// string is a guid to the resource
// if resources were loaded on a level by level basis, then resources could be ref counted.
// or, for simplicity, every resource is a "global" resource
// that is to say the resource is loaded when the game starts, and is unloaded when the game ends.

class iResource;

typedef std::unordered_map<std::string, iResource*> tResourceMap;

class iResourceManager
{
public:
	virtual void LoadResource(const std::string& id, const std::string& path) = 0;
	iResource* GetResource(const std::string& id);
	void UnloadResources();

protected:
	tResourceMap m_resourceMap;

};

class TextureManager : public iResourceManager
{
public:
	void LoadResource(const std::string& id, const std::string& path) override;

};

class MeshManager : public iResourceManager
{
public:
	void LoadResource(const std::string& id, const std::string& path) override;

};

class SoundManager : public iResourceManager
{
public:
	void LoadResource(const std::string& id, const std::string& path) override;

};

#pragma once

#include <map>
#include <string>

class Texture;

typedef std::map<std::string, Texture*> tTextureMap;

class TextureManager
{
public:
	TextureManager() {}
	~TextureManager() {}

	void UnloadResources();

	bool LoadTexture(const std::string& name, const std::string& path);

	Texture* GetTexture(const std::string& name);

private:
	tTextureMap m_textureMap;

};

extern TextureManager gTextureManager;

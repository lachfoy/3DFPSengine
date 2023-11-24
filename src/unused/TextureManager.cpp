#include "Common.h"
#include "TextureManager.h"

#include <iostream>

#include "Texture.h"
TextureManager gTextureManager;

void TextureManager::UnloadResources()
{
	std::cout << "Unloading textures\n";
	std::cout << "--------------------------------------------------------\n";
	for (auto& it : m_textureMap)
	{
		delete it.second;
		std::cout << "Unloaded texture: " << it.first << "\n";
	}
	std::cout << "--------------------------------------------------------\n\n";
}

bool TextureManager::LoadTexture(const std::string& name, const std::string& path)
{
	Texture* texture = new Texture();
	bool ret = texture->LoadFromFile(path);

	m_textureMap[name] = texture;

	return ret;
}

Texture* TextureManager::GetTexture(const std::string& name)
{
	Texture* temp = m_textureMap.at(name);
	ASSERT_MSG(temp, "Missing texture \"%s\"", name.c_str());
	return temp;
}
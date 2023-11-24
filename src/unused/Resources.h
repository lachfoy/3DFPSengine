#pragma once

class TextureManager;
class MeshManager;
class SoundManager;

class Resources
{
public:
	Resources() {};
	~Resources() = default;

	void Init();
	void Unload();

	TextureManager* GetTextureManager() { return m_textureManager; }
	MeshManager* GetMeshManager() { return m_meshManager; }
	SoundManager* GetSoundManager() { return m_soundManager; }

private:
	TextureManager* m_textureManager;
	MeshManager* m_meshManager;
	SoundManager* m_soundManager;

};
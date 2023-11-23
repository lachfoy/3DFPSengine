#pragma once

class SoundManager;

class Resources
{
public:
	Resources() {};
	~Resources() = default;

	void Init();

	SoundManager* GetSoundManager() { return m_soundManager; }

private:
	SoundManager* m_soundManager;

};
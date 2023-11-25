#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <string>

class Sound
{
public:
	Sound();
	~Sound();

	static Sound* LoadFromFile(const std::string& path);

	void Play() const;

	void SetVolume(float volume);

private:
	ALuint m_buffer;
	ALuint m_source;

};

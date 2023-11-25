#include "Sound.h"

#include <sndfile.h>

Sound::Sound()
{
	alGenBuffers(1, &m_buffer);
	alGenSources(1, &m_source);
}

Sound::~Sound()
{
	alDeleteSources(1, &m_source);
	alDeleteBuffers(1, &m_buffer);
}

Sound* Sound::LoadFromFile(const std::string& path)
{
	SNDFILE* file;
	SF_INFO info;

	file = sf_open(path.c_str(), SFM_READ, &info);
	if (!file)
	{
		printf("Error loading sound\n");
		return nullptr;
	}

	float* data = new float[info.frames];
	if (!data)
	{
		printf("Error allocating memory for sound\n");
		sf_close(file);
		return nullptr;
	}

	sf_count_t numFrames = sf_read_float(file, data, info.frames);
	if (numFrames != info.frames)
	{
		printf("Read unexpected number of frames in sound file\n");
	}

	Sound* sound = new Sound();

	ALenum format = NULL;
	if (info.channels == 1)
	{
		format = AL_FORMAT_MONO16;
	}
	else if (info.channels == 2)
	{
		format = AL_FORMAT_STEREO16;
	}

	alBufferData(sound->m_buffer, format, data, info.frames, info.samplerate);
	alSourcei(sound->m_source, AL_BUFFER, sound->m_buffer);

	sf_close(file);
	delete[] data;

	return sound;
}

void Sound::Play() const
{
	alSourcePlay(m_source);
}

void Sound::SetVolume(float volume)
{
	alSourcef(m_source, AL_GAIN, volume);
}

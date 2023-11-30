#include "AudioEngine.h"

#include <glm/gtc/type_ptr.hpp>

AudioEngine gAudioEngine;

void AudioEngine::Init()
{
	m_device = alcOpenDevice(nullptr); // open default device
	if (!m_device)
	{
		/* handle error */
		printf("AL error \n");

	}

	m_context = alcCreateContext(m_device, nullptr);
	if (!m_context || !alcMakeContextCurrent(m_context))
	{
		/* handle error */
		printf("AL error \n");

	}


	// Generate buffers
	alGetError(); // Clear error code

	alGenBuffers(NUM_BUFFERS, m_buffers);
	ALenum err = alGetError();
	if (err != AL_NO_ERROR)
	{
		alDeleteBuffers(NUM_BUFFERS, m_buffers);
		printf("AL error \n");
	}

}

void AudioEngine::Destroy()
{
	printf("DELETING SOUND BUFFERS\n");
	alDeleteBuffers(NUM_BUFFERS, m_buffers);

	alcMakeContextCurrent(nullptr);
	alcDestroyContext(m_context);
	alcCloseDevice(m_device);
}

void AudioEngine::SetMasterGain(float gain)
{
	alListenerf(AL_GAIN, gain);

	ALenum err = alGetError();
	if (err != AL_NO_ERROR)
	{
		alDeleteBuffers(NUM_BUFFERS, m_buffers);
		printf("AL error \n");
	}
}

void AudioEngine::SetListenerPosition(const glm::vec3& position)
{
	alListenerfv(AL_POSITION, glm::value_ptr(position));

	ALenum err = alGetError();
	if (err != AL_NO_ERROR)
	{
		alDeleteBuffers(NUM_BUFFERS, m_buffers);
		printf("AL error \n");
	}
}

void AudioEngine::SetListenerVelocity(const glm::vec3& velocity)
{
	alListenerfv(AL_VELOCITY, glm::value_ptr(velocity));

	ALenum err = alGetError();
	if (err != AL_NO_ERROR)
	{
		alDeleteBuffers(NUM_BUFFERS, m_buffers);
		printf("AL error \n");
	}
}

void AudioEngine::SetListenerOrientation(const glm::vec3& front, const glm::vec3& up)
{
	ALfloat orientation[] = {
		front.x, front.y, front.z,
		up.x, up.y, up.z
	};

	alListenerfv(AL_ORIENTATION, orientation);

	ALenum err = alGetError();
	if (err != AL_NO_ERROR)
	{
		alDeleteBuffers(NUM_BUFFERS, m_buffers);
		printf("AL error \n");
	}
}

void AudioEngine::LoadSound()
{
	ALuint sourceHandle;
	alGenSources(1, &sourceHandle);

	ALenum err = alGetError();
	if (err != AL_NO_ERROR)
	{
		printf("AL error \n");
	}

	alSourcei(sourceHandle, AL_BUFFER, m_buffers[0]);
}

int AudioEngine::Play3DSound(const glm::vec3& position)
{
	return 0;
}

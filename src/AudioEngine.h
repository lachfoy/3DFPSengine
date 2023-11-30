#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include <glm/glm.hpp>
#include "NonCopyable.h"

#define NUM_BUFFERS 16

class AudioEngine : public NonCopyable
{
public:
	AudioEngine() {}

	void Init();
	void Destroy();

	void SetMasterGain(float gain);
	void SetListenerPosition(const glm::vec3& position);
	void SetListenerVelocity(const glm::vec3& velocity);
	void SetListenerOrientation(const glm::vec3& front, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

	void LoadSound();
	int Play3DSound(const glm::vec3& position); // returns handle to sound

private:
	ALCdevice* m_device;
	ALCcontext* m_context;

	ALuint m_buffers[NUM_BUFFERS];

};

extern AudioEngine gAudioEngine;

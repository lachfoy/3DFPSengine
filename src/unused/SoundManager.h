#pragma once

#include "iResourceManager.h"

class iResource;

class SoundManager : public iResourceManager
{
public:
	SoundManager() {}
	~SoundManager() {}

	iResource* GetSound(const std::string& name);

private:


};

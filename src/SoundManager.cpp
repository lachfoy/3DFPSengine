#include "Common.h"
#include "SoundManager.h"

iResource* SoundManager::GetSound(const std::string& name)
{
	iResource* temp = m_resourceMap.at(name);
	ASSERT_MSG(temp, "Missing sound \"%s\"", name.c_str());
	return temp;
}

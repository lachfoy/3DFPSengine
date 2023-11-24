#include "Resources.h"

#include "SoundManager.h"

void Resources::Init()
{
	m_soundManager = new SoundManager();


}

void Resources::Unload()
{
	m_soundManager->UnloadResources();
}

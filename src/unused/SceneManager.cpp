#include "Common.h"
#include "SceneManager.h"

SceneManager gSceneManager;

void SceneManager::GoToScene(std::unique_ptr<Scene> newScene)
{
	ASSERT(newScene);

	if (m_currentScene)
	{
		m_currentScene->Destroy();
	}

	newScene->Create();

	m_currentScene = std::move(newScene);
}

void SceneManager::FixedUpdate()
{
	if (m_currentScene)
	{
		m_currentScene->FixedUpdate();
	}
}

void SceneManager::Update(float dt)
{
	if (m_currentScene)
	{
		m_currentScene->Update(dt);
	}
}

void SceneManager::Render()
{
	if (m_currentScene)
	{
		m_currentScene->Render();
	}
}

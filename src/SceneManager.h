#pragma once

#include "Scene.h"
#include <memory>

class SceneManager
{
public:
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	void GoToScene(std::unique_ptr<Scene> newScene);
	
	void FixedUpdate();
	void Update(float dt);
	void Render();

private:
	SceneManager() = default;
	~SceneManager() = default;

	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

	std::unique_ptr<Scene> m_currentScene;

};

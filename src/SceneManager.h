#pragma once

#include "NonCopyable.h"
#include "Scene.h"
#include <memory>

class SceneManager : public NonCopyable
{
public:
	SceneManager() = default;
	~SceneManager() = default;

	void GoToScene(std::unique_ptr<Scene> newScene);
	const std::unique_ptr<Scene>& GetCurrentScene() const { return m_currentScene; }

	void FixedUpdate();
	void Update(float dt);
	void Render();

private:
	std::unique_ptr<Scene> m_currentScene;

};

extern SceneManager gSceneManager;

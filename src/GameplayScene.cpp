#include "Common.h"
#include "GameplayScene.h"

#include "ResourceManager.h"
#include "Texture.h"
#include "Sound.h"
#include "Player.h"
#include "Camera.h"
#include "PhysicsWorld.h"
#include "Renderer.h"
#include "Enemy.h"
#include "Input.h"

void GameplayScene::Create()
{
	ResourceManager::Instance().LoadResource<Texture>("data/images/round_cat.png", "cat");
	ResourceManager::Instance().LoadResource<Texture>("data/images/missing.png", "missing");
	ResourceManager::Instance().LoadSound("pew", "data/sounds/pew.wav");

	ResourceManager::Instance().GetSound("pew")->SetGain(1.0f);

	m_camera = new FirstPersonCamera();
	m_player = new Player(glm::vec3(0.f, 5.0f, 0.0f), *m_camera);
	m_camera->UpdateProjection(static_cast<float>(800) / static_cast<float>(600)); // need to get this from a window class...

	// Todo - refactor level object to include its own collision, rather than creating seperately
	m_level = new Level();
	gRenderer.AddToRenderList(m_level);
	gPhysicsWorld.CreateStaticLevelGeometry("data/models/test.obj");

	Enemy* enemy = new Enemy(glm::vec3(0.0f, 10.0f, 0.0f), m_player);
	gRenderer.AddToRenderList(enemy);
	m_entities.push_back(enemy);
}

void GameplayScene::FixedUpdate()
{
	m_player->FixedUpdate();

	for (Entity* entity : m_entities)
	{
		entity->FixedUpdate();
	}
}


void GameplayScene::Update(float dt)
{
	if (gInput.IsKeyPressed(SDL_SCANCODE_Z))
	{
		CatCube* catCube = gPhysicsWorld.AddCatCube(glm::vec3(0, 5.0f, 0)); // these never get deleted but thats ok ig
		gRenderer.AddToRenderList(catCube);
	}

	// TODO!! HANDLE FROM WINDOW CLASS
	//if (gInput.IsKeyPressed(SDL_SCANCODE_0))
	//{
	//	ScreenshotManager::TakeScreenshot(m_windowWidth, m_windowHeight);
	//}

	//if (gInput.IsKeyPressed(SDL_SCANCODE_F))
	//{
	//	// This just switches to fullscreen. But can't switch back (lol)
	//	// We need to save the windowed configuration to switch back to it.
	//	SDL_DisplayMode displayMode;
	//	SDL_GetDesktopDisplayMode(0, &displayMode); // can return int error

	//	m_windowWidth = displayMode.w;
	//	m_windowHeight = displayMode.h;

	//	m_viewportWidth = m_windowWidth / 2;
	//	m_viewportHeight = m_windowHeight / 2;

	//	SDL_SetWindowSize(m_window, m_windowWidth, m_windowHeight);
	//	SDL_SetWindowBordered(m_window, SDL_FALSE);

	//	glViewport(0, 0, m_windowWidth, m_windowHeight);

	//	m_player->GetCamera()->UpdateProjection(static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight));
	//	gTextRenderer.SetProjection(m_viewportWidth, m_viewportHeight);
	//}

	m_player->Update(dt);

	for (Entity* entity : m_entities)
	{
		entity->Update(dt);
	}
}

void GameplayScene::Render()
{

}

void GameplayScene::Destroy()
{
	for (Entity* entity : m_entities)
	{
		SAFE_DELETE(entity);
	}

	SAFE_DELETE(m_player);
	SAFE_DELETE(m_level);
	SAFE_DELETE(m_camera);
}

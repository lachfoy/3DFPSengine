#include "Common.h"
#include "GameplayScene.h"

#include "Global.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Mesh.h"
#include "Sound.h"
#include "Player.h"
#include "Camera.h"
#include "PhysicsWorld.h"
#include "Renderer.h"
#include "DebugRenderer.h"
#include "Enemy.h"
#include "Input.h"

GameplayScene::GameplayScene()
{
	ResourceManager::Instance().LoadResource<Texture>("data/images/round_cat.png", "cat");
	ResourceManager::Instance().LoadResource<Texture>("data/images/missing.png", "missing");
	ResourceManager::Instance().LoadSound("pew", "data/sounds/pew.wav");
	ResourceManager::Instance().LoadResource<Mesh>("data/models/test.obj", "level");
	ResourceManager::Instance().LoadResource<Mesh>("data/models/cube.obj", "cube");
	ResourceManager::Instance().GetSound("pew")->SetGain(1.0f);

	m_activeCamera = new FirstPersonCamera();
	m_player = new Player(glm::vec3(0.f, 5.0f, 0.0f), *m_activeCamera);
	m_activeCamera->SetAspect(static_cast<float>(1280) / static_cast<float>(720)); // need to get this from a window class...

	// Todo - refactor level object to include its own collision, rather than creating seperately
	m_level = new Level();

	Enemy* enemy = new Enemy(glm::vec3(0.0f, 10.0f, 0.0f), m_player);
	AddEntity(enemy);
}

GameplayScene::~GameplayScene()
{
	printf("Gameplay scene destructor\n");

	SAFE_DELETE(m_player);
	SAFE_DELETE(m_level);
	SAFE_DELETE(m_activeCamera);
}

void GameplayScene::FixedUpdate()
{
	m_player->FixedUpdate();

	for (auto it : m_entities)
	{
		if (it.second)
		it.second->FixedUpdate();
	}
}

void GameplayScene::Update(float dt)
{
	m_player->Update(dt);

	for (auto it : m_entities)
	{
		if (it.second)
		it.second->Update(dt);
	}
}

void GameplayScene::Render()
{
	m_level->Render();

	for (auto it : m_entities)
	{
		if (it.second)
			it.second->Render();
	}

	if (m_activeCamera)
	{
		gRenderer.Render(m_activeCamera);
		gDebugRenderer.Render(m_activeCamera);
	}
}

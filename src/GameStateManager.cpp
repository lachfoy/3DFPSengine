#include "GameStateManager.h"

#include "iGameState.h"

void GameStateManager::HandleInput(Input* input)
{
	if (m_currentGameState)
	{
		m_currentGameState->HandleInput(input);
	}
}

void GameStateManager::FixedUpdate()
{
	if (m_currentGameState)
	{
		m_currentGameState->FixedUpdate();
	}
}

void GameStateManager::Update(float dt)
{
	if (m_currentGameState)
	{
		m_currentGameState->Update(dt);
	}
}

void GameStateManager::Render()
{
	if (m_currentGameState)
	{
		m_currentGameState->Render();
	}
}

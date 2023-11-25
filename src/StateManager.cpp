#include "StateManager.h"

#include "iState.h"

void StateManager::FixedUpdate()
{
	if (m_currentState)
	{
		m_currentState->FixedUpdate();
	}
}

void StateManager::Update(float dt)
{
	if (m_currentState)
	{
		m_currentState->Update(dt);
	}
}

void StateManager::Render()
{
	if (m_currentState)
	{
		m_currentState->Render();
	}
}

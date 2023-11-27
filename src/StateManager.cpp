#include "Common.h"
#include "StateManager.h"

void StateManager::GoToState(std::unique_ptr<iState> nextState)
{
	ASSERT(nextState);

	if (m_currentState)
	{
		m_currentState->Destroy();
	}

	nextState->Create();

	m_currentState = std::move(nextState);
}

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

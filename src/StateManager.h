#pragma once

#include "iState.h"
#include <memory>

class StateManager
{
public:
	static StateManager& Instance()
	{
		static StateManager instance;
		return instance;
	}

	void GoToState(std::unique_ptr<iState> nextState);
	
	void FixedUpdate();
	void Update(float dt);
	void Render();

private:
	StateManager() = default;
	~StateManager() = default;

	StateManager(const StateManager&) = delete;
	StateManager& operator=(const StateManager&) = delete;

	std::unique_ptr<iState> m_currentState;

};

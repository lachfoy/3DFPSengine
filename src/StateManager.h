#pragma once

class iState;
class Input;

class StateManager
{
public:
	static StateManager& Instance()
	{
		static StateManager instance;
		return instance;
	}

	void FixedUpdate();
	void Update(float dt);
	void Render();

private:
	StateManager() = default;

	StateManager(const StateManager&) = delete;
	StateManager& operator=(const StateManager&) = delete;

	iState* m_currentState;

};

#pragma once

class iGameState;
class Input;

class GameStateManager
{
public:
	static GameStateManager& Instance()
	{
		static GameStateManager instance;
		return instance;
	}

	void HandleInput(Input* input);
	void Update(float dt);
	void FixedUpdate();
	void Render();

private:
	GameStateManager() = default;

	GameStateManager(const GameStateManager&) = delete;
	GameStateManager& operator=(const GameStateManager&) = delete;

	iGameState* m_currentGameState;

};

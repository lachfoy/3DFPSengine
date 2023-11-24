#pragma once

#include "iGameState.h"

class Input;

class GameplayState : public iGameState
{
public:
	GameplayState() {}
	~GameplayState() = default;

	void Create() override;
	
	void HandleInput(Input* input) {}
	void FixedUpdate() {}
	void Update(float dt) {}
	void Render() {}
	
	void Destroy() override;

protected:

};

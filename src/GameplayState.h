#pragma once

#include "iState.h"

class GameplayState : public iState
{
public:
	GameplayState() {}
	~GameplayState() = default;

	void Create() override;
	
	void FixedUpdate() override {}
	void Update(float dt) override {}
	void Render() override {}
	
	void Destroy() override;

protected:

};

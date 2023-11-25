#pragma once

#include "iState.h"

class GameplayState : public iState
{
public:
	GameplayState() {}
	~GameplayState() = default;

	void Create() override;
	
	void FixedUpdate() {}
	void Update(float dt) {}
	void Render() {}
	
	void Destroy() override;

protected:

};

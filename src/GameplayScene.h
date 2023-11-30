#pragma once

#include "Scene.h"

class GameplayState : public Scene
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

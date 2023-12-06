#pragma once

#include "Scene.h"

class Player;
class Level;
class Entity;

#include <vector>

class GameplayScene : public Scene
{
public:
	GameplayScene() = default;
	~GameplayScene() = default;

	void Create() override;
	
	void FixedUpdate() override;
	void Update(float dt) override;

	void Destroy() override;

protected:
	Player* m_player;
	Level* m_level;

};

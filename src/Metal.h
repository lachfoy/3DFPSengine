#pragma once

#include "SpriteEntity.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

class Input;
class Player;

class Metal : public SpriteEntity
{
public:
	Metal(glm::vec2 position, Player* player);
	~Metal() {}

	void Think(); // exact same as enemy
	void Update(float dt);

private:
	Player* m_player;

	float m_acceleration = 1000.0f;

	glm::vec2 m_velocity{ 0.0f };
	float kFrictionCoef = 8.f;

	float m_thinkTimer = 0.0f;
	const float kThinkInterval = 0.5f;

	bool m_active = false;
	float m_activationRadius = 80.0f; // this really shouldnt be on metal, but should be on the player

};
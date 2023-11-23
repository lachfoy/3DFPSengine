#pragma once

#include "IRenderable.h"

#include <glad/glad.h>
#include <glm/glm.hpp>


class Input;
class Player;

class btKinematicCharacterController;

class Enemy : public iRenderable
{
public:
	Enemy(const glm::vec3& position, Player* player);
	~Enemy() {}

	void Damage(int amount);
	int GetDamage() const { return rand() % (m_maxDamage - m_minDamage + 1) + m_minDamage; }

	void Think();
	void OnUpdate(float dt) override;

	void OnDestroy() override;

private:
	glm::vec3 m_moveDir{ 0.0f };
	
	float m_acceleration = 400.0f;

	glm::vec3 m_velocity{ 0.0f };
	float kFrictionCoef = 8.f;

	float m_thinkTimer = 0.0f;
	const float kThinkInterval = 0.5f;

	int m_maxHealth = 10;
	int m_health = m_maxHealth;

	int m_minDamage = 1;
	int m_maxDamage = 5;

	btKinematicCharacterController* m_characterController;

	Player* m_player; // this is stupid. Rename the class to player, not FirstPersonController

};
#pragma once

#include "Renderable.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

class Input;
class Player;

class btKinematicCharacterController;

class Enemy : public Renderable
{
public:
	Enemy(const glm::vec3& position, Player* player);
	~Enemy();

	void Damage(int amount);
	int GetDamage() const { return rand() % (m_maxDamage - m_minDamage + 1) + m_minDamage; }

	void FixedUpdate() override;
	void Update(float dt) override;

private:
	int m_maxHealth = 10;
	int m_health = m_maxHealth;

	int m_minDamage = 1;
	int m_maxDamage = 5;

	btKinematicCharacterController* m_characterController;

	Player* m_player;

	glm::vec3 m_walkDirection;
	float m_walkSpeed = 0.01f;

};
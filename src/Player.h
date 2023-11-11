#pragma once

#include "IRenderable.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

class Input;

class Player : public IRenderable
{
public:
	Player();
	~Player() {}

	void HandleInput(Input* input);

	void Damage(int amount);
	bool CanTakeDamage() const { return !m_immune; }

	void Shoot();

	void OnUpdate(float dt) override;

private:
	glm::vec3 m_moveDir{ 0.0f };
	float m_acceleration = 800.0f;

	glm::vec3 m_velocity{ 0.0f };
	float kFrictionAmount = 8.f;
	float kMaxSpeed = 400.0f;

	int m_maxHealth = 100;
	int m_health = m_maxHealth;

	float m_immuneTimer = 0.0f;
	const float kImmuneInterval = 0.5f;
	bool m_immune = false;

};
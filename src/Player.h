#pragma once

#include "SpriteEntity.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "Projectile.h"

class Projectile;//???? why do i need this forward declare? I dont understand

class Input;

class Player : public SpriteEntity
{
public:
	Player(glm::vec2 position, std::vector<std::unique_ptr<Projectile>>* projectiles);
	~Player() {}

	void HandleInput(Input* input);
	void Update(float dt);

	void Render(SpriteRenderer* renderer) override;

	void Damage(int amount);
	bool CanTakeDamage() const { return !m_immune; }

	void AddMetal(int amount) { m_metalCount += amount; }

	void Shoot();

private:
	glm::vec2 m_moveDir{ 0.0f };
	float m_acceleration = 800.0f;

	glm::vec2 m_velocity{ 0.0f };
	float kFrictionAmount = 8.f;
	float kMaxSpeed = 400.0f;

	SpriteEntity m_shotgun;
	int m_startingMetalCount = 50;
	int m_metalCount = m_startingMetalCount;

	int m_maxHealth = 100;
	int m_health = m_maxHealth;

	float m_immuneTimer = 0.0f;
	const float kImmuneInterval = 0.5f;
	bool m_immune = false;

	float m_shotSpread = 10.0f; // DEGREES!
	int m_numShots = 5;

	glm::vec2 m_aimTarget;

	std::vector<std::unique_ptr<Projectile>>* m_projectiles;

};
#pragma once

#include "SpriteEntity.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "Enemy.h"
#include "Projectile.h"

class Enemy;

class Turret : public SpriteEntity
{
public:
	Turret(glm::vec2 position, std::vector<Enemy*>& enemies, std::vector<std::unique_ptr<Projectile>>* projectiles);
	~Turret() {}

	void Think(); // exact same as enemy
	void Update(float dt) override;

	void Damage(int amount);

	void Render(SpriteRenderer* renderer) override;

private:
	std::vector<Enemy*>& m_enemies;

	float m_thinkTimer = 0.0f;
	const float kThinkInterval = 0.5f;

	float m_detectRadius = 100.0f;

	int m_targetEnemyIndex = -1;

	std::vector<std::unique_ptr<Projectile>>* m_projectiles;

	float m_shotTimer = 0.0f;
	const float kShotInterval = 0.2f;
	bool m_canShoot = true;

	int m_maxHealth = 10;
	int m_health = m_maxHealth;

};
#include "Enemy.h"

#include "Input.h"

#include "DebugRenderer.h"
#include "TextureManager.h"
#include "Player.h"

#include "World.h"

Enemy::Enemy(glm::vec2 position)
	: Entity()
{
}

void Enemy::Damage(int amount)
{
	m_health -= amount;
	printf("enemy damaged for %d. New hp = %d/%d\n", amount, m_health, m_maxHealth);

	if (m_health <= 0)
	{
		printf("Enemy destroyed\n");
	}
}

void Enemy::Think()
{
	
}

void Enemy::OnUpdate(float dt)
{
	m_thinkTimer += dt;
	if (m_thinkTimer >= kThinkInterval)
	{
		m_thinkTimer = 0.0f;
		Think();
	}



	// Apply movement
	m_velocity -= m_velocity * kFrictionCoef * dt;
	m_velocity += m_acceleration * m_moveDir * dt;

	m_worldPosition += m_velocity * dt;
}

void Enemy::OnDestroy()
{
	
}

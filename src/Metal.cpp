#include "Metal.h"

#include "SpriteRenderer.h"
#include "DebugRenderer.h"
#include "Player.h"
#include "TextureManager.h"

Metal::Metal(glm::vec2 position, Player* player)
	: SpriteEntity(position, glm::vec2(8, 8), gTextureManager.GetTexture("diamond")), m_player(player)
{
}

void Metal::Think()
{
	float distance = glm::length(m_player->GetPosition() - m_position);
	if (distance <= m_activationRadius)
	{
		m_active = true;
	}
}

void Metal::Update(float dt)
{
	m_thinkTimer += dt;
	if (m_thinkTimer >= kThinkInterval)
	{
		m_thinkTimer = 0.0f;
		Think();
	}

	if (!m_active) return;

	glm::vec2 moveDir = glm::normalize(m_player->GetPosition() - m_position);

	// Apply movement
	m_velocity -= m_velocity * kFrictionCoef * dt;
	m_velocity += m_acceleration * moveDir * dt;

	m_position += m_velocity * dt;
}

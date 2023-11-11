#include "Player.h"

#include "Common.h"
#include "Input.h"

#include "DebugRenderer.h"
#include "TextureManager.h"

#include "TextRenderer.h"

Player::Player() : Entity()
{
	
}

void Player::HandleInput(Input* input)
{
	if (input->IsKeyHeld(SDL_SCANCODE_W) || input->IsKeyHeld(SDL_SCANCODE_UP)) {
		m_moveDir.y = -1.0f;
	}
	if (input->IsKeyHeld(SDL_SCANCODE_A) || input->IsKeyHeld(SDL_SCANCODE_LEFT)) {
		m_moveDir.x = -1.0f;
	}
	if (input->IsKeyHeld(SDL_SCANCODE_S) || input->IsKeyHeld(SDL_SCANCODE_DOWN)) {
		m_moveDir.y = 1.0f;
	}
	if (input->IsKeyHeld(SDL_SCANCODE_D) || input->IsKeyHeld(SDL_SCANCODE_RIGHT)) {
		m_moveDir.x = 1.0f;
	}
}

void Player::Update(float dt)
{
	if (m_immune) // replace this with a timer class probably
	{
		m_immuneTimer += dt;
		if (m_immuneTimer >= kImmuneInterval)
		{
			m_immuneTimer = 0.0f;
			m_immune = false;
		}
	}

	if (glm::length(m_moveDir) > 0.0f)
	{
		m_moveDir = glm::normalize(m_moveDir);
	}


	// Apply movement
	//float damping = pow(1.0f - kFrictionAmount, dt); // Calculate the damping factor based on friction coefficient and elapsed time
	//m_velocity *= damping;

	m_velocity -= m_velocity * kFrictionAmount * dt; // Apply friction first
	m_velocity += m_acceleration * m_moveDir * dt;

	m_worldPosition += m_velocity * dt;

	m_moveDir = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Player::Damage(int amount)
{
	if (m_health > 0)
	{
		m_health -= amount;
		printf("Player took %d damage! New hp = %d/%d\n", amount, m_health, m_maxHealth);

		if (m_health <= 0)
		{
			printf("Dead!!!!!\n");
		}
		else
		{
			// kick immunity timer
			m_immune = true;
		}
	}
}

void Player::Shoot()
{

}


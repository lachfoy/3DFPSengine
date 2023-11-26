#include "Enemy.h"

#include "Input.h"

#include "DebugRenderer.h"
#include "ResourceManager.h"
#include "Player.h"

#include "Common.h"

#include "Mesh.h"
#include <glm/gtc/matrix_transform.hpp>

#include "PhysicsWorld.h"

Enemy::Enemy(const glm::vec3& position, Player* player)
	: m_player(player)
{
	m_mesh = Mesh::CreateQuad(glm::vec2(1.0f, 2.8f));

	m_texture = ResourceManager::Instance().GetTexture("cat");

	m_transform = glm::translate(glm::mat4(1.0f), position);

	m_characterController = gPhysicsWorld.CreateCharacter(position);
	m_characterController->getGhostObject()->setUserPointer((void*)this);
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

void Enemy::FixedUpdate()
{
	if (glm::length(m_walkDirection) > 0.0f)
	{
		m_walkDirection = glm::normalize(m_walkDirection) * m_walkSpeed;
	}
	else
	{
		m_walkDirection = glm::vec3(0.0f);
	}

	btVector3 btWalkDirection(m_walkDirection.x, m_walkDirection.y, m_walkDirection.z);
	m_characterController->setWalkDirection(btWalkDirection);
}

void Enemy::Update(float dt)
{
	btTransform btWorldTransform = m_characterController->getGhostObject()->getWorldTransform();
	btVector3 origin = btWorldTransform.getOrigin();

	m_position = glm::vec3(origin.x(), origin.y(), origin.z());

	glm::vec3 playerPos = m_player->GetPosition();

	glm::vec3 dirToPlayer = glm::normalize(playerPos - m_position);

	dirToPlayer.y = 0.0f;
	m_walkDirection = dirToPlayer;

	float radiansToPlayer = atan2(dirToPlayer.x, dirToPlayer.z);
	m_rotation.y = radiansToPlayer;

	UpdateTransform();
}

void Enemy::Destroy()
{
	
}

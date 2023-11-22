#include "Enemy.h"

#include "Input.h"

#include "DebugRenderer.h"
#include "TextureManager.h"
#include "Player.h"

#include "Common.h"

#include "World.h"
#include "Mesh.h"
#include <glm/gtc/matrix_transform.hpp>

#include "PhysicsWorld.h"

Enemy::Enemy(const glm::vec3& position, Player* player)
	: m_player(player)
{
	m_mesh = Mesh::CreateQuad(glm::vec2(1.0f, 2.8f));

	m_texture = gTextureManager.GetTexture("cat");

	m_transform = glm::translate(glm::mat4(1.0f), position);

	m_characterController = gPhysicsWorld.CreateCharacter(position);
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
	btTransform btWorldTransform = m_characterController->getGhostObject()->getWorldTransform();
	btVector3 origin = btWorldTransform.getOrigin();

	m_worldPosition = glm::vec3(origin.x(), origin.y(), origin.z());

	glm::vec3 playerPos = m_player->GetPosition();

	glm::vec3 dirToPlayer = glm::normalize(playerPos - m_worldPosition);

	//dirToPlayer.y = 0.0f;
	//glm::vec3 walkDirection = dirToPlayer * 5.0f * dt;
	//btVector3 btWalkDirection(walkDirection.x, walkDirection.y, walkDirection.z);
	//m_characterController->setWalkDirection(btWalkDirection);

	//float radiansToPlayer =  atan2(dirToPlayer.x, dirToPlayer.z);
	//m_rotation.y = radiansToPlayer; // Welp. Rotations are not correct. Only works when rotating around the origin

	UpdateTransform();
}

void Enemy::OnDestroy()
{
	
}

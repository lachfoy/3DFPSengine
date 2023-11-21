#include "Enemy.h"

#include "Input.h"

#include "DebugRenderer.h"
#include "TextureManager.h"
#include "Player.h"

#include "World.h"
#include "Mesh.h"
#include <glm/gtc/matrix_transform.hpp>

#include "PhysicsWorld.h"

Enemy::Enemy(const glm::vec3& position)
{
	m_mesh = Mesh::CreateMeshFromFile("data/models/cube.obj");

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

	m_rotation.y += dt * 2.0f;

	UpdateTransform();
}

void Enemy::OnDestroy()
{
	
}

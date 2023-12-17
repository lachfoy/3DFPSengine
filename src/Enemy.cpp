#include "Enemy.h"

#include "Input.h"

#include "DebugRenderer.h"
#include "ResourceManager.h"
#include "Player.h"

#include "Common.h"

#include "Mesh.h"
#include <glm/gtc/matrix_transform.hpp>

#include "PhysicsWorld.h"
#include "Scene.h"

Enemy::Enemy(const glm::vec3& position, Player* player)
	: m_player(player)
{
	m_mesh = Mesh::CreateQuad(glm::vec2(1.0f, 2.8f));

	m_texture = ResourceManager::Instance().GetResource<Texture>("cat");

	m_transform = glm::translate(glm::mat4(1.0f), position);


}

Enemy::~Enemy()
{
	delete m_characterController;
}

void Enemy::Damage(int amount)
{
	m_health -= amount;
	printf("enemy with id %d damaged for %d. New hp = %d/%d\n", m_id, amount, m_health, m_maxHealth);

	if (m_health <= 0)
	{
		m_scene->RemoveEntity(this); // ENSURE THE COLLISION INFORMATION IS ALSO DELETED
		// fuck
	}
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


}

void Enemy::Update(float dt)
{

	glm::vec3 playerPos = m_player->GetPosition();

	glm::vec3 dirToPlayer = glm::normalize(playerPos - m_position);

	dirToPlayer.y = 0.0f;
	m_walkDirection = dirToPlayer;

	float radiansToPlayer = atan2(dirToPlayer.x, dirToPlayer.z);
	m_rotation.y = radiansToPlayer;

	UpdateTransform();
}

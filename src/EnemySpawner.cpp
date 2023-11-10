#include "EnemySpawner.h"

#include "Common.h"
#include "Player.h"

#include "World.h"

EnemySpawner::EnemySpawner(Player* player) : m_player(player)
{
}

void EnemySpawner::Update(float dt)
{
	//m_elapsedTime += dt;
	m_spawnTimer += dt;

	if (m_spawnTimer >= m_spawnInterval)
	{
		SpawnEnemyGroup();
		m_spawnTimer = 0.0f; // Reset
	}
}

void EnemySpawner::SpawnEnemyGroup()
{
	glm::vec2 playerPos = m_player->GetPosition();

	for (int i = 0; i < m_enemiesPerGroup; i++)
	{
		float randomAngle = static_cast<float>(rand()) / RAND_MAX * 2.0f * PI;
		glm::vec2 spawnPos = playerPos + glm::vec2(std::cos(randomAngle), std::sin(randomAngle)) * m_spawnRadius;

		gWorld.AddSpriteEntity(new Enemy(spawnPos));
	}
}

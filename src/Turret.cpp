#include "Turret.h"

#include "SpriteRenderer.h"
#include "DebugRenderer.h"
#include "TextureManager.h"

#include "TextRenderer.h"

Turret::Turret(glm::vec2 position, std::vector<Enemy*>& enemies, std::vector<std::unique_ptr<Projectile>>* projectiles)
	: SpriteEntity(position, glm::vec2(16, 16), gTextureManager.GetTexture("turret")), m_enemies(enemies), m_projectiles(projectiles)
{
	gDebugRenderer.AddCircle(position, m_detectRadius, glm::vec3(1.0f, 0.0f, 1.0f)); //whar
}

void Turret::Think()
{
	if (m_targetEnemyIndex != -1) return; // dont bother picking a new target if the current one is still being tracked

	//if (!m_enemies->empty())
	//{
	//	std::vector<size_t> enemiesInRangeIndices;
	//	for (size_t i = 0; i < m_enemies->size(); ++i)
	//	{
	//		if ((*m_enemies)[i] && !(*m_enemies)[i]->GetRemove())
	//		{
	//			float dist = glm::length((*m_enemies)[i]->GetPosition() - m_position);
	//			if (dist < m_detectRadius)
	//			{
	//				enemiesInRangeIndices.push_back(i);
	//			}
	//		}
	//	}
	//
	//	if (!enemiesInRangeIndices.empty())
	//	{
	//		int randomIndex = rand() % enemiesInRangeIndices.size();
	//		m_targetEnemyIndex = enemiesInRangeIndices[randomIndex];
	//	}
	//	else
	//	{
	//		m_targetEnemyIndex = -1;
	//	}
	//}
	//else
	//{
	//	m_targetEnemyIndex = -1;
	//}
}

void Turret::Update(float dt)
{
	m_thinkTimer += dt;
	if (m_thinkTimer >= kThinkInterval)
	{
		m_thinkTimer = 0.0f;
		Think();
	}

	if (!m_canShoot)
		m_shotTimer += dt;

	if (m_shotTimer >= kShotInterval)
	{
		m_shotTimer = 0.0f;
		m_canShoot = true;
	}

	//if (m_targetEnemyIndex != -1)
	//{
	//	if (m_targetEnemyIndex >= m_enemies->size() || (*m_enemies)[m_targetEnemyIndex] == nullptr || (*m_enemies)[m_targetEnemyIndex]->GetRemove())
	//	{
	//		// Target is dead, reset target index
	//		m_targetEnemyIndex = -1;
	//		return;
	//	}

	//	std::unique_ptr<Enemy>& enemy = (*m_enemies)[m_targetEnemyIndex];
	//	glm::vec2 vecToEnemy = enemy->GetPosition() - m_position;
	//	float dist = glm::length(vecToEnemy);

	//	if (dist > m_detectRadius)
	//	{
	//		// Target went out of range, reset target index
	//		m_targetEnemyIndex = -1;
	//		return;
	//	}

	//	glm::vec2 directionToEnemy = glm::normalize(vecToEnemy);
	//	float angleToEnemy = atan2(directionToEnemy.y, directionToEnemy.x);
	//	SetRotation(angleToEnemy);

	//	if (m_canShoot)
	//	{
	//		gDebugRenderer.AddLine(m_position, enemy->GetPosition(), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f);
	//		m_projectiles->push_back(std::make_unique<Projectile>(m_position, directionToEnemy, ProjectileType::SHOT));
	//		m_canShoot = false;
	//	}
	//}
}

void Turret::Damage(int amount)
{
	if (m_health > 0)
	{
		m_health -= amount;

		if (m_health <= 0)
		{
			printf("Dead!!!!!\n");
		}
		else
		{
			// kick immunity timer
			//m_immune = true;
		}
	}
}

void Turret::Render(SpriteRenderer* renderer)
{
	SpriteEntity::Render(renderer);

	gTextRenderer.AddStringToBatch(std::to_string(m_health), m_position.x, m_position.y, glm::vec3(1.0f));
}

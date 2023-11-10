#pragma once

#include "SpriteEntity.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

enum class ProjectileType
{
	SHOT,
	REPAIR_CLAW, //4:1 health to metal ratio; in effect, healing 4 health consumes 1 unit of metal.
};

class Projectile : public SpriteEntity
{
public:
	Projectile(glm::vec2 position, glm::vec2 direction);
	Projectile(glm::vec2 position, glm::vec2 direction, ProjectileType type);
	~Projectile() {}

	void Update(float dt);

private:
	
	glm::vec2 m_moveDir{0.0f};
	float m_moveSpeed = 300.0f;

	float m_lifeTime = 1.0f;

};

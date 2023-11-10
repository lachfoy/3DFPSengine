#include "World.h"

#include "Projectile.h"

World gWorld;

World::World()
{

}

World::~World()
{
	for (const auto& spriteEntity : m_spriteEntities)
	{
		delete spriteEntity;
	}
}

void World::AddSpriteEntity(SpriteEntity* spriteEntity)
{
	m_spriteEntities.push_back(spriteEntity);
}

void World::AddToDeleteList(SpriteEntity* spriteEntity)
{
	m_deleteList.push_back(spriteEntity);
}

void World::CleanUpDeleteList()
{

}

void World::UpdateEntities(float dt)
{
	for (const auto& spriteEntity : m_spriteEntities)
	{
		spriteEntity->Update(dt);
	}
}

void World::RenderEntities(SpriteRenderer* renderer)
{
	for (const auto& spriteEntity : m_spriteEntities)
	{
		spriteEntity->Render(renderer);
	}
}
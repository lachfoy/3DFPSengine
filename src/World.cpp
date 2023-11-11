#include "World.h"

#include "Entity.h"

World gWorld;

World::World()
{

}

World::~World()
{
	for (Entity* entity : m_entities)
	{
		delete entity;
	}
}

void World::AddEntity(Entity* Entity)
{
	m_entities.push_back(Entity);
}

void World::AddToDeleteList(Entity* Entity)
{
	m_deleteList.push_back(Entity);
}

void World::CleanUpDeleteList()
{

}

void World::UpdateEntities(float dt)
{
	for (Entity* entity : m_entities)
	{
		entity->OnUpdate(dt);
	}
}
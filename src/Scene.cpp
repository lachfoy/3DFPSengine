#include "Scene.h"

#include "Renderer.h"
#include "DebugRenderer.h"
#include "Entity.h"

Scene::~Scene()
{
	printf("Scene Base destructor\n");

	for (const auto it : m_entities)
	{
		it.second->Destroy();
		delete it.second;
	}
}

void Scene::AddEntity(Entity* entity)
{
	if (entity)
	{
		entity->Create();
		entity->m_scene = this;
		m_entities[entity->m_id] = entity;
	}
}

void Scene::RemoveEntity(Entity* entity)
{
	if (entity)
	{
		printf("Destroying entity with id: %d\n", entity->m_id);
		entity->Destroy();
		m_entities.erase(entity->m_id);
		delete entity;
	}
}

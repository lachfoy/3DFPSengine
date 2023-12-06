#include "Scene.h"

#include "Renderer.h"
#include "DebugRenderer.h"
#include "Entity.h"

void Scene::Render()
{
	for (auto it : m_entities)
	{
		if (it.second)
			it.second->Render();
	}

	if (m_activeCamera)
	{
		gRenderer.Render(m_activeCamera);
		gDebugRenderer.Render(m_activeCamera);
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
		delete entity; // Use unique pointers for this?
	}
}

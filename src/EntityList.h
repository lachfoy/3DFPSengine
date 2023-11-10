#pragma once

#include <iterator>
#include <list>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Entity;

class SpriteRenderer;

class IEntityListener;

struct EntityInfo
{
	Entity* m_entity;
	EntityInfo* m_next;
	EntityInfo* m_prev;
};


#define NUM_ENTITIES 128

class EntityList
{
public:
	EntityList();
	~EntityList();

	Entity* GetNextEntity(Entity* current);

	void AddEntity(Entity* entity); // pass ownership
	void RemoveEntity();

	// Events
	void AddListener(IEntityListener* listener);

	void NotifyCreateEntity(Entity* entity);
	void NotifySpawn(Entity* entity);
	void NotifyRemoveEntity(Entity* entity);

	void AddToDeleteList(Entity* entity);
	void CleanUpDeleteList();

	Entity* FindEntityInRadius(Entity* start, const glm::vec2& position, float radius);

	void UpdateEntities(float dt);
	void DrawEntities(SpriteRenderer* renderer);

private:
	EntityInfo* m_head;
	EntityInfo m_entityInfoPtrArray[NUM_ENTITIES]; // array of infos

	int m_lastIndex = 0;

	// maybe not use these if we do a fancy linked list from scratch?
	std::list<Entity*> m_entityList;
	std::list<Entity*> m_entityDeleteList;
	std::list<IEntityListener*> m_entityListeners;

};

extern EntityList gEntityList;

class IEntityListener
{
public:
	virtual void OnEntityCreated(Entity* entity) {};
	virtual void OnEntitySpawned(Entity* entity) {};
	virtual void OnEntityDeleted(Entity* entity) {};

};

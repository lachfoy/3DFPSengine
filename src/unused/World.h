#pragma once

//#include <iterator>
#include <list>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

class TextureManager;
class SpriteRenderer;

class Entity;

class World
{
public:
	World();
	~World();

	void AddEntity(Entity* Entity); // pass ownership

	const std::list<Entity*>& GetEntities() const { return m_entities; }

	Entity* GetPlayer() { return nullptr; }
	const std::list<Entity*>& GetEnemies() const { return m_entities; }
	const std::list<Entity*>& GetPickups() const { return m_entities; }
	const std::list<Entity*>& GetTurrets() const { return m_entities; }

	void AddToDeleteList(Entity* Entity);
	void CleanUpDeleteList();

	void UpdateEntities(float dt);

private:
	void UpdateEnemies(float dt);
	void UpdatePickups(float dt);
	void UpdateTurrets(float dt);

	std::list<Entity*> m_entities;


	std::list<Entity*> m_deleteList;



};

extern World gWorld;

#pragma once

//#include <iterator>
#include <list>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

class TextureManager;
class SpriteRenderer;

class SpriteEntity;

class World
{
public:
	World();
	~World();

	void AddSpriteEntity(SpriteEntity* spriteEntity); // pass ownership

	const std::list<SpriteEntity*>& GetSpriteEntities() const { return m_spriteEntities; }

	SpriteEntity* GetPlayer() { return nullptr; }
	const std::list<SpriteEntity*>& GetEnemies() const { return m_enemies; }
	const std::list<SpriteEntity*>& GetPickups() const { return m_pickups; }
	const std::list<SpriteEntity*>& GetTurrets() const { return m_turrets; }

	void AddToDeleteList(SpriteEntity* spriteEntity);
	void CleanUpDeleteList();

	void UpdateEntities(float dt);
	void RenderEntities(SpriteRenderer* renderer);

private:
	void UpdateEnemies(float dt);
	void UpdatePickups(float dt);
	void UpdateTurrets(float dt);
	void RenderEnemies(SpriteRenderer* renderer);
	void RenderPickups(SpriteRenderer* renderer);
	void RenderTurrets(SpriteRenderer* renderer);

	SpriteRenderer* m_spriteRenderer; // non owning
	TextureManager* m_textureManager; // non owning

	std::list<SpriteEntity*> m_spriteEntities;

	std::list<SpriteEntity*> m_enemies;
	std::list<SpriteEntity*> m_pickups;
	std::list<SpriteEntity*> m_turrets;

	std::list<SpriteEntity*> m_deleteList;



};

extern World gWorld;

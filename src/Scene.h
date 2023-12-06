#pragma once

#include <unordered_map>

class Camera;
class Entity;

class Scene
{
public:
	Scene() {}
	~Scene() = default;

	virtual void Create() = 0;
	
	virtual void FixedUpdate() {}
	virtual void Update(float dt) {}
	void Render(); // this actually handles talking to the renderer
	virtual void Destroy() = 0;

	void AddEntity(Entity* entity);
	void RemoveEntity(Entity* entity);

protected:
	Camera* m_activeCamera;

	// In more complex engines, the scene maintains a "scene graph"
	// The scene graph is a data structure used to organise the scene objects
	// This is used for rendering efficiently (skipping occluded objects for example)

	std::unordered_map<int, Entity*> m_entities; // Entities, stored with an ID

	// store a seperate list of enemies, projectiles, etc?

};

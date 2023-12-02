#pragma once

class Camera;

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

protected:
	Camera* m_activeCamera;

	// In more complex engines, the scene maintains a "scene graph"
	// The scene graph is a data structure used to organise the scene objects
	// This is used for rendering efficiently (skipping occluded objects for example)

};

#pragma once

class Camera;

class Scene
{
public:
	Scene() {}
	~Scene() = default;

	Camera* GetCamera() const { return m_camera; }

	virtual void Create() = 0;
	
	virtual void FixedUpdate() {}
	virtual void Update(float dt) {}
	virtual void Render() {}
	
	virtual void Destroy() = 0;

protected:
	Camera* m_camera;

};

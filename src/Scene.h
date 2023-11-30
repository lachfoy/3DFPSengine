#pragma once

class Scene
{
public:
	Scene() {}
	~Scene() = default;

	virtual void Create() = 0;
	
	virtual void FixedUpdate() {}
	virtual void Update(float dt) {}
	virtual void Render() {}
	
	virtual void Destroy() = 0;

};

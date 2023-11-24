#pragma once

class Input;

class iGameState
{
public:
	iGameState() {}
	~iGameState() = default;

	virtual void Create() = 0;
	
	virtual void HandleInput(Input* input) {}
	virtual void FixedUpdate() {}
	virtual void Update(float dt) {}
	virtual void Render() {}
	
	virtual void Destroy() = 0;

protected:


};

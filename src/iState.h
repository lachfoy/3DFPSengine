#pragma once

class iState
{
public:
	iState() {}
	~iState() = default;

	virtual void Create() = 0;
	
	virtual void FixedUpdate() {}
	virtual void Update(float dt) {}
	virtual void Render() {}
	
	virtual void Destroy() = 0;

protected:


};

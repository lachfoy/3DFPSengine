#pragma once

#include "Camera.h"

class Input;
class btKinematicCharacterController;

class FirstPersonController : public Camera
{
public:
	FirstPersonController(btKinematicCharacterController* characterController);
	~FirstPersonController() = default;

	void HandleInput(Input* input);
	void PhysicsUpdate(float dt);
	void Update(float dt);

private:
	btKinematicCharacterController* m_characterController;

	glm::vec3 m_walkDirection;
	float m_walkSpeed = 10.0f;

};

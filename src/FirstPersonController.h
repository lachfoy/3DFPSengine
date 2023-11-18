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
	float m_walkSpeed = 6.0f;
	float m_jumpAmount = 10.0f;
	float m_cameraYOffsetFromOrigin = 0.9f; // // this makes the total height of the camera 1.4

};

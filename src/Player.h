#pragma once

#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>

class Input;
//class btKinematicCharacterController;
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
class Camera;

class Player
{
public:
	Player(btKinematicCharacterController* characterController);
	~Player() = default;

	void HandleInput(Input* input);
	void FixedUpdate();
	void Update(float dt);

	Camera* GetCamera() const { return m_camera; }

	glm::vec3 GetPosition() const
	{
		btVector3 btPosition = m_characterController->getGhostObject()->getWorldTransform().getOrigin(); // ugh
		return glm::vec3(btPosition.x(), btPosition.y(), btPosition.z());
	}

private:
	btKinematicCharacterController* m_characterController;

	Camera* m_camera;

	glm::vec3 m_walkDirection;
	float m_walkSpeed = 0.16f;
	float m_jumpAmount = 7.0f;
	float m_cameraYOffsetFromOrigin = 0.9f; // // this makes the total height of the camera 1.4

	bool m_jumpInput = false;

};

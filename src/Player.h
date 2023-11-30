#pragma once

#include "Entity.h"
#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>

//class btKinematicCharacterController;
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
class Camera;

class Player : public Entity
{
public:
	Player(const glm::vec3& position, Camera& camera);
	~Player() = default;

	void FixedUpdate() override;
	void Update(float dt) override;

	glm::vec3 GetPosition() const
	{
		btVector3 btPosition = m_characterController->getGhostObject()->getWorldTransform().getOrigin(); // ugh
		return glm::vec3(btPosition.x(), btPosition.y(), btPosition.z());
	}

private:
	btKinematicCharacterController* m_characterController;

	Camera& m_camera; // NON OWNING

	glm::vec3 m_walkDirection;
	float m_walkSpeed = 0.16f;
	float m_jumpAmount = 7.0f;
	float m_cameraYOffsetFromOrigin = 0.9f; // // this makes the total height of the camera 1.4

};

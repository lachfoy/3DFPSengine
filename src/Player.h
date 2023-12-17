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
	~Player();

	void FixedUpdate() override;
	void Update(float dt) override;

private:
	Camera& m_camera; // NON OWNING

	glm::vec3 m_walkDirection;
	float m_walkSpeed = 6.0f;
	float m_jumpAmount = 7.0f;
	float m_cameraYOffsetFromOrigin = 0.9f; // // this makes the total height of the camera 1.4

	btCapsuleShape* m_capsuleShape;
	btCollisionObject* m_collisionObject;

};

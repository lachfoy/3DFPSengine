#include "FirstPersonController.h"

#include "Input.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include <string>
#include "TextRenderer.h"

#include <glm/gtc/type_ptr.hpp>

FirstPersonController::FirstPersonController(btKinematicCharacterController* characterController)
	: Camera(glm::vec3(0.0f)), m_characterController(characterController)
{
}

void FirstPersonController::HandleInput(Input* input)
{
	m_walkDirection = glm::vec3(0.0f);
	if (input->IsKeyHeld(SDL_SCANCODE_W) || input->IsKeyHeld(SDL_SCANCODE_UP))
	{
		m_walkDirection += glm::vec3(m_front.x, 0.0f, m_front.z);
	}
	if (input->IsKeyHeld(SDL_SCANCODE_S) || input->IsKeyHeld(SDL_SCANCODE_DOWN))
	{
		m_walkDirection -= glm::vec3(m_front.x, 0.0f, m_front.z);
	}
	if (input->IsKeyHeld(SDL_SCANCODE_A) || input->IsKeyHeld(SDL_SCANCODE_LEFT))
	{
		m_walkDirection -= glm::vec3(m_right.x, 0.0f, m_right.z);
	}
	if (input->IsKeyHeld(SDL_SCANCODE_D) || input->IsKeyHeld(SDL_SCANCODE_RIGHT))
	{
		m_walkDirection += glm::vec3(m_right.x, 0.0f, m_right.z);
	}

	if (input->IsKeyPressed(SDL_SCANCODE_SPACE) && m_characterController->onGround())
	{
		m_characterController->jump(btVector3(0.0f, m_jumpAmount, 0.0f));
	}

	// Mouse movement
	glm::vec2 mouseRelPos = input->GetMouseRelPos();
	float xOffset = mouseRelPos.x * m_mouseSensitivity;
	float yOffset = -mouseRelPos.y * m_mouseSensitivity;

	m_yaw += xOffset;
	m_pitch += yOffset;
}

void FirstPersonController::PhysicsUpdate(float dt)
{
	if (glm::length(m_walkDirection) > 0.0f)
	{
		m_walkDirection = glm::normalize(m_walkDirection);
	}

	glm::vec3 walkDirection = m_walkDirection * m_walkSpeed * dt;
	btVector3 btWalkDirection(walkDirection.x, walkDirection.y, walkDirection.z);
	m_characterController->setWalkDirection(btWalkDirection);

	std::string debugString;
	debugString += "walk dir:" + std::to_string(m_walkDirection.x) + ", " + std::to_string(m_walkDirection.y) + ", " + std::to_string(m_walkDirection.z) + "\n"; // position is camera position
	gTextRenderer.AddStringToBatch(debugString, 0, 0, glm::vec3(1.0f));
}

void FirstPersonController::Update(float dt)
{
	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (m_pitch > 89.0f)
	{
		m_pitch = 89.0f;
	}

	if (m_pitch < -89.0f)
	{
		m_pitch = -89.0f;
	}

	UpdateCameraVectors();

	// make the camera actually follow the character controller
	btTransform btWorldTransform = m_characterController->getGhostObject()->getWorldTransform();
	btVector3 origin = btWorldTransform.getOrigin();

	m_position = glm::vec3(origin.x(), origin.y() + m_cameraHeight, origin.z());
}

#include "Player.h"

#include "Input.h"


#include <string>
#include "TextRenderer.h"

#include <glm/gtc/type_ptr.hpp>


#include "Camera.h"

#include "PhysicsWorld.h"
#include "ResourceManager.h"
#include "Sound.h"
#include "DebugRenderer.h"
#include "Enemy.h"
#include "AudioEngine.h"

Player::Player(btKinematicCharacterController* characterController)
	: m_characterController(characterController)
{
	m_characterController->getGhostObject()->setUserPointer((void*)this);
	m_camera = new FirstPersonCamera();
}

void Player::FixedUpdate()
{
	if (glm::length(m_walkDirection) > 0.0f)
	{
		m_walkDirection = glm::normalize(m_walkDirection) * m_walkSpeed;
	}
	else
	{
		m_walkDirection = glm::vec3(0.0f);
	}

	btVector3 btWalkDirection(m_walkDirection.x, m_walkDirection.y, m_walkDirection.z);
	m_characterController->setWalkDirection(btWalkDirection);

	//std::string debugString;
	//debugString += "walk dir:" + std::to_string(m_walkDirection.x) + ", " + std::to_string(m_walkDirection.y) + ", " + std::to_string(m_walkDirection.z) + "\n"; // position is camera position
	//gTextRenderer.AddStringToBatch(debugString, 0, 0, glm::vec3(1.0f));
}

void Player::Update(float dt)
{
	m_walkDirection = glm::vec3(0.0f);

	glm::vec3 front = m_camera->GetFront();
	glm::vec3 right = m_camera->GetRight();

	if (Input::Instance().IsKeyHeld(SDL_SCANCODE_W) || Input::Instance().IsKeyHeld(SDL_SCANCODE_UP))
	{
		m_walkDirection += glm::vec3(front.x, 0.0f, front.z);
	}
	if (Input::Instance().IsKeyHeld(SDL_SCANCODE_S) || Input::Instance().IsKeyHeld(SDL_SCANCODE_DOWN))
	{
		m_walkDirection -= glm::vec3(front.x, 0.0f, front.z);
	}
	if (Input::Instance().IsKeyHeld(SDL_SCANCODE_A) || Input::Instance().IsKeyHeld(SDL_SCANCODE_LEFT))
	{
		m_walkDirection -= glm::vec3(right.x, 0.0f, right.z);
	}
	if (Input::Instance().IsKeyHeld(SDL_SCANCODE_D) || Input::Instance().IsKeyHeld(SDL_SCANCODE_RIGHT))
	{
		m_walkDirection += glm::vec3(right.x, 0.0f, right.z);
	}

	if (Input::Instance().IsKeyPressed(SDL_SCANCODE_SPACE) && m_characterController->onGround())
	{
		m_characterController->jump(btVector3(0.0f, m_jumpAmount, 0.0f));
	}

	if (Input::Instance().IsMouseButtonPressed(SDL_BUTTON_LEFT))
	{
		ResourceManager::Instance().GetSound("pew")->Play(); // pew
		
		// Raycast from center of camera

		glm::vec3 from = m_camera->GetPosition();

		// Convert glm::vec3 to btVector3
		btVector3 btFrom(from.x, from.y, from.z);
		btVector3 btTo = btFrom + btVector3(front.x, front.y, front.z) * 50.0f;

		// Perform raycast
		btCollisionWorld::ClosestRayResultCallback rayCallback(btFrom, btTo);
		gPhysicsWorld.GetDynamicsWorld()->rayTest(btFrom, btTo, rayCallback);
		
		if (rayCallback.hasHit())
		{
			btVector3 btHit = rayCallback.m_hitPointWorld;
			glm::vec3 hit = glm::vec3(btHit.x(), btHit.y(), btHit.z());

			gDebugRenderer.AddLine(from, hit, glm::vec3(0.0f, 1.0f, 1.0f), 2.0f);
			gDebugRenderer.AddBox(hit, glm::vec3(0.1f), glm::vec3(0.0f, 1.0f, 1.0f), 2.0f);

			// Ray hit something
			if (rayCallback.m_collisionObject)
			{
				Enemy* enemy = static_cast<Enemy*>(rayCallback.m_collisionObject->getUserPointer());
				if (enemy)
				{
					printf("Hit enemy\n");
				}

				// Check if the collision was static geometry
				const btBroadphaseProxy* proxy = rayCallback.m_collisionObject->getBroadphaseHandle();
				if (proxy)
				{
					if (proxy->m_collisionFilterGroup == btBroadphaseProxy::StaticFilter)
					{
						printf("Hit static geometry\n");
					}
				}
			}
		}
	}

	m_camera->Update(dt);

	// make the camera actually follow the character controller
	btTransform btWorldTransform = m_characterController->getGhostObject()->getWorldTransform();

	btVector3 origin = btWorldTransform.getOrigin();

	glm::vec3 cameraPosition = glm::vec3(origin.x(), origin.y() + m_cameraYOffsetFromOrigin, origin.z());
	m_camera->SetPosition(cameraPosition);

	gAudioEngine.SetListenerPosition(cameraPosition);
	gAudioEngine.SetListenerOrientation(front);

	std::string debugString;
	debugString += "origin:" + std::to_string(origin.x()) + ", " + std::to_string(origin.y()) + ", " + std::to_string(origin.z()) + "\n";
	gTextRenderer.AddStringToBatch(debugString, 0, 0, glm::vec3(1.0f));
}

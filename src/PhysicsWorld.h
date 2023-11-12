#pragma once

#include <btBulletDynamicsCommon.h>


#include "BulletDebugDraw.h"

#include <glm/glm.hpp>

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include <vector>

#include "IRenderable.h"

class TestBox : public IRenderable
{
public:
	TestBox(btRigidBody* body);
	~TestBox();

	void UpdateTransform();

private:
	btRigidBody* m_rigidBody;

};

class PhysicsWorld
{
public:
	PhysicsWorld();
	~PhysicsWorld();

	void stepSimulation(float timeStep, int maxSubSteps = 10);

	void addRigidBody(btRigidBody* body);
	void removeRigidBody(btRigidBody* body);

	btRigidBody* addBox(const glm::vec3& halfExtents, float mass, const glm::mat4& startTransform);

	TestBox* AddTestBox(const glm::vec3& position);


	void CreateCharacter();

	void DebugDraw();

private:
	btDefaultCollisionConfiguration* m_collisionConfiguration;

	btDiscreteDynamicsWorld* m_dynamicsWorld;

	btBroadphaseInterface* m_broadphase;

	btCollisionDispatcher* m_dispatcher; // can derive this to add events to collisions

	btSequentialImpulseConstraintSolver* m_solver;
	
	BulletDebugDraw* m_debugDrawer;


	btKinematicCharacterController* m_character; // temp

	std::vector<btCollisionShape*> m_collisionShapes;

};

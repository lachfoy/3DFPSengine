#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include "BulletDebugDraw.h"

#include <glm/glm.hpp>

class PhysicsWorld
{
public:
	PhysicsWorld();
	~PhysicsWorld();

	void stepSimulation(float timeStep, int maxSubSteps = 10);

	void addRigidBody(btRigidBody* body);
	void removeRigidBody(btRigidBody* body);

	btRigidBody* addBox(const glm::vec3& halfExtents, float mass, const glm::mat4& startTransform);

	void Render();

private:
	btDiscreteDynamicsWorld* m_dynamicsWorld;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btCollisionDispatcher* m_dispatcher;
	btBroadphaseInterface* m_overlappingPairCache;
	btSequentialImpulseConstraintSolver* m_solver;
	BulletDebugDraw* m_debugDrawer;

};

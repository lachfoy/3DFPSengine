#pragma once

#include <btBulletDynamicsCommon.h>


#include "BulletDebugDraw.h"

#include <glm/glm.hpp>

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include <vector>

#include "IRenderable.h"

class CatCube : public IRenderable, public btMotionState
{
public:
	CatCube(const glm::vec3& position);
	~CatCube() = default;

	void getWorldTransform(btTransform& worldTrans) const override;

	//Bullet only calls the update of worldtransform for active objects
	void setWorldTransform(const btTransform& worldTrans) override;

private:
	//btRigidBody* m_rigidBody; // doesnt need this tbh

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

	CatCube* AddCatCube(const glm::vec3& position);


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

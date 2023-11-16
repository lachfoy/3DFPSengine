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

class Level : public IRenderable
{
public:
	Level();
	~Level() = default;

};

class PhysicsWorld
{
public:
	PhysicsWorld();
	~PhysicsWorld();

	void StepSimulation(float timeStep, int maxSubSteps = 10);

	CatCube* AddCatCube(const glm::vec3& position);

	btKinematicCharacterController* CreateCharacter();

	void CreateStaticLevelGeometry(std::string pathToObj);


	void DebugDraw();

private:
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btDiscreteDynamicsWorld* m_dynamicsWorld;
	btBroadphaseInterface* m_broadphase;
	btCollisionDispatcher* m_dispatcher; // can derive this to add events to collisions
	btSequentialImpulseConstraintSolver* m_solver;
	BulletDebugDraw* m_debugDrawer;

	std::vector<btCollisionObject*> m_collisionObjects;
	std::vector<btActionInterface*> m_actions;
	std::vector<btCollisionShape*> m_collisionShapes;
	std::vector<btMotionState*> m_motionStates;

	btTriangleMesh* m_mesh; // see if this gets deleted

};

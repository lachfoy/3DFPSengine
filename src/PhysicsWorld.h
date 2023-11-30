#pragma once

#include <btBulletDynamicsCommon.h>

#include "BulletDebugDraw.h"

#include <glm/glm.hpp>

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include <vector>

#include "Renderable.h"
#include "NonCopyable.h"

class CatCube : public Renderable, public btMotionState
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

class Level : public Renderable
{
public:
	Level();
	~Level() = default;

};

// Setup a collision detection callback
struct MyContactCallback : public btCollisionWorld::ContactResultCallback
{
	bool m_collisionDetected;

	MyContactCallback() : m_collisionDetected(false) {}

	btScalar addSingleResult(btManifoldPoint& cp,
		const btCollisionObjectWrapper* colObj0, int partId0, int index0,
		const btCollisionObjectWrapper* colObj1, int partId1, int index1) override
	{
		m_collisionDetected = true;
		return 0; // Return 0 to ignore this collision pair in the future
	}
};

class PhysicsWorld : public NonCopyable
{
public:
	PhysicsWorld();
	~PhysicsWorld();

	PhysicsWorld(const PhysicsWorld&) = delete;
	PhysicsWorld& operator=(const PhysicsWorld&) = delete;

	btDiscreteDynamicsWorld* GetDynamicsWorld() const { return m_dynamicsWorld; }

	void StepSimulation(float timeStep, int maxSubSteps = 10);

	CatCube* AddCatCube(const glm::vec3& position);

	btKinematicCharacterController* CreateCharacter(const glm::vec3& position);

	void CreateStaticLevelGeometry(const std::string& pathToObj);

	bool ExpensiveAABBTestVsLevelGeometry(const glm::vec3& position, const glm::vec3& halfExtents);

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

	btTriangleMesh* m_mesh; // see if this gets deleted (it doesnt..)
	btGhostPairCallback* m_ghostPairCallback; //see if this gets deleted (it doesnt..)

	btRigidBody* m_levelCollisionObject; // added to m_collisionObjects, so no need to delete

};

extern PhysicsWorld gPhysicsWorld;

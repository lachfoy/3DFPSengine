#pragma once

#include <btBulletDynamicsCommon.h>

#include "BulletDebugDraw.h"

#include <glm/glm.hpp>

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include <vector>
#include <memory>
#include "Mesh.h"
#include "Texture.h"

class Level
{
public:
	Level();
	~Level();
	
	void Render();

private:
	glm::mat4 m_transform;

	std::shared_ptr<Mesh> m_mesh;
	std::shared_ptr<Texture> m_texture;

	btTriangleMesh* m_collisionMesh;
	btBvhTriangleMeshShape* m_meshShape;
	btCollisionObject* m_collisionObject;

};


struct CollisionData
{
	glm::vec3 contactNormal;
	float penetrationDepth;
	// Other relevant data
};


class MyContactResultCallback : public btCollisionWorld::ContactResultCallback {
public:
	std::vector<CollisionData> collisions;

	bool HasCollision() const { return m_hasCollision; }

	virtual btScalar addSingleResult(btManifoldPoint& cp,
		const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0,
		const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override {

		m_hasCollision = true;
		
		// Process collision point
		btVector3 contactPoint = cp.getPositionWorldOnA();
		btVector3 contactNormal = cp.m_normalWorldOnB;
		btScalar penetrationDepth = cp.getDistance();

		// Your collision handling logic here
		CollisionData data;
		data.contactNormal = glm::vec3(contactNormal.x(), contactNormal.y(), contactNormal.z());
		data.penetrationDepth = penetrationDepth;
		collisions.push_back(data);

		return 0;
	}

private:
	bool m_hasCollision = false;

};



class PhysicsWorld
{
public:
	PhysicsWorld();
	~PhysicsWorld();

	PhysicsWorld(const PhysicsWorld&) = delete;
	PhysicsWorld& operator=(const PhysicsWorld&) = delete;

	btCollisionWorld* GetCollisionWorld() const { return m_collisionWorld; }
	
	void ResolveCollisions();
	
	void DebugDraw();




private:
	btBroadphaseInterface* m_broadphase;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btCollisionDispatcher* m_dispatcher; // can derive this to add events to collisions

	btCollisionWorld* m_collisionWorld;
	BulletDebugDraw* m_debugDrawer;

};

extern PhysicsWorld gPhysicsWorld;

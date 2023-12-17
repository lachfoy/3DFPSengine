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

class PhysicsWorld
{
public:
	PhysicsWorld();
	~PhysicsWorld();

	PhysicsWorld(const PhysicsWorld&) = delete;
	PhysicsWorld& operator=(const PhysicsWorld&) = delete;

	btCollisionWorld* GetCollisionWorld() const { return m_collisionWorld; }
	void DebugDraw();

private:
	btBroadphaseInterface* m_broadphase;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btCollisionDispatcher* m_dispatcher; // can derive this to add events to collisions

	btCollisionWorld* m_collisionWorld;
	BulletDebugDraw* m_debugDrawer;

};

extern PhysicsWorld gPhysicsWorld;

#include "PhysicsWorld.h"

#include <glm/gtc/type_ptr.hpp>
#include "Common.h"

#include "ResourceManager.h"

#include "DebugRenderer.h"

#include "Renderer.h"

Level::Level()
{
	m_mesh = ResourceManager::Instance().GetResource<Mesh>("level");
	m_texture = ResourceManager::Instance().GetResource<Texture>("missing");
	m_transform = glm::mat4(1.0f);

	m_collisionMesh = Mesh::LoadCollisionMeshFromOBJ("data/models/test.obj");

	bool useQuantizedAABBCompression = true;
	m_meshShape = new btBvhTriangleMeshShape(m_collisionMesh, useQuantizedAABBCompression);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(0, 0, 0)); // Position the mesh accordingly

	m_collisionObject = new btCollisionObject();

	m_collisionObject->setWorldTransform(transform);
	m_collisionObject->setCollisionShape(m_meshShape);

	gPhysicsWorld.GetCollisionWorld()->addCollisionObject(m_collisionObject,
		btBroadphaseProxy::StaticFilter, btBroadphaseProxy::DefaultFilter);
}

Level::~Level()
{
	gPhysicsWorld.GetCollisionWorld()->removeCollisionObject(m_collisionObject);

	if (m_collisionObject)
	{
		delete m_collisionObject;
		m_collisionObject = nullptr;
	}

	if (m_meshShape)
	{
		delete m_meshShape;
		m_meshShape = nullptr;
	}

	if (m_collisionMesh)
	{
		delete m_collisionMesh;
		m_collisionMesh = nullptr;
	}
}

void Level::Render()
{
	gRenderer.AddToRenderList(m_mesh, m_transform, m_texture);
}

PhysicsWorld gPhysicsWorld;

PhysicsWorld::PhysicsWorld()
{
	m_broadphase = new btDbvtBroadphase();
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

	m_collisionWorld = new btCollisionWorld(m_dispatcher, m_broadphase, m_collisionConfiguration);

	m_debugDrawer = new BulletDebugDraw();
	m_collisionWorld->setDebugDrawer(m_debugDrawer);
}

PhysicsWorld::~PhysicsWorld()
{
	printf("Cleaning up physics world\n");
	printf("--------------------------------------------------------\n");

	printf("Deleting collision world...\n");
	SAFE_DELETE(m_collisionWorld);

	printf("Deleting broadphase...\n");
	SAFE_DELETE(m_broadphase);

	printf("Deleting dispatcher...\n");
	SAFE_DELETE(m_dispatcher);

	printf("Deleting collision configuration...\n");
	SAFE_DELETE(m_collisionConfiguration);

	printf("Deleting debug drawer...\n");
	SAFE_DELETE(m_debugDrawer);

	printf("--------------------------------------------------------\n\n");
}

void PhysicsWorld::DebugDraw()
{
	m_collisionWorld->debugDrawWorld();
}

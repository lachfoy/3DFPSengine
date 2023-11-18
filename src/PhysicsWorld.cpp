#include "PhysicsWorld.h"

#include <glm/gtc/type_ptr.hpp>
#include "Common.h"

#include "TextureManager.h"
#include "Mesh.h"

CatCube::CatCube(const glm::vec3& position)
{
	m_mesh = Mesh::CreateMeshFromFile("data/models/cube.obj");

	m_texture = gTextureManager.GetTexture("cat");

	m_transform = glm::translate(glm::mat4(1.0f), position);
}

void CatCube::getWorldTransform(btTransform& worldTrans) const
{
	worldTrans.setFromOpenGLMatrix(glm::value_ptr(m_transform));
}

void CatCube::setWorldTransform(const btTransform& worldTrans)
{
	float mat[16];
	worldTrans.getOpenGLMatrix(mat);
	m_transform = glm::make_mat4(mat);
}

Level::Level()
{
	m_mesh = Mesh::CreateMeshFromFile("data/models/level.obj");

	m_texture = gTextureManager.GetTexture("missing");

	m_transform = glm::mat4(1.0f);
}

PhysicsWorld::PhysicsWorld()
{
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	m_broadphase = new btDbvtBroadphase();
	m_solver = new btSequentialImpulseConstraintSolver;
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

	m_dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));  // Set gravity as needed

	m_debugDrawer = new BulletDebugDraw();
	m_dynamicsWorld->setDebugDrawer(m_debugDrawer);

	/// create a ground plane
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	m_collisionShapes.push_back(groundShape);

	// Create a default motion state (position and rotation)
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	m_motionStates.push_back(groundMotionState);

	// Create the rigid body construction info (mass = 0 => static body)
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));

	// Create the rigid body
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

	// Add the ground plane rigid body to the dynamics world
	int groundCollisionGroup = btBroadphaseProxy::StaticFilter;
	int groundCollisionMask = btBroadphaseProxy::AllFilter;
	m_dynamicsWorld->addRigidBody(groundRigidBody, groundCollisionGroup, groundCollisionMask);
	m_collisionObjects.push_back(groundRigidBody);
}

PhysicsWorld::~PhysicsWorld()
{
	printf("Cleaning up physics world\n");
	printf("--------------------------------------------------------\n");
	
	printf("Deleting collision objects...\n");
	for (btCollisionObject* collisionObject : m_collisionObjects)
	{
		m_dynamicsWorld->removeCollisionObject(collisionObject);
		SAFE_DELETE(collisionObject);
	}

	printf("Deleting actions...\n");
	for (btActionInterface* action : m_actions)
	{
		m_dynamicsWorld->removeAction(action);
		SAFE_DELETE(action);
	}

	printf("Deleting collision shapes...\n");
	for (btCollisionShape* collisionShape : m_collisionShapes)
	{
		SAFE_DELETE(collisionShape);
	}

	printf("Deleting motion states...\n");
	for (btMotionState* motionState : m_motionStates)
	{
		SAFE_DELETE(motionState);
	}

	printf("Deleting dynamics world...\n");
	SAFE_DELETE(m_dynamicsWorld);

	printf("Deleting solver...\n");
	SAFE_DELETE(m_solver);

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

void PhysicsWorld::StepSimulation(float timeStep, int maxSubSteps)
{
	m_dynamicsWorld->stepSimulation(timeStep, maxSubSteps);
}

CatCube* PhysicsWorld::AddCatCube(const glm::vec3& position)
{
	btScalar mass = 1.0f;
	btVector3 btHalfExtents(0.5f, 0.5f, 0.5f);

	// Create the box shape
	btCollisionShape* boxShape = new btBoxShape(btHalfExtents);
	m_collisionShapes.push_back(boxShape);

	// Calculate the local inertia
	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
	{
		boxShape->calculateLocalInertia(mass, localInertia);
	}

	CatCube* catCube = new CatCube(position);
	m_motionStates.push_back(catCube);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, catCube, boxShape, localInertia);

	// Create the rigid body
	btRigidBody* body = new btRigidBody(rbInfo); // I have no idea if dynamics world handles this memory

	// Add the body to the dynamics world
	m_dynamicsWorld->addRigidBody(body);
	m_collisionObjects.push_back(body);

	return catCube;
}

btKinematicCharacterController* PhysicsWorld::CreateCharacter()
{
	btPairCachingGhostObject* ghostObject = new btPairCachingGhostObject();
	btScalar characterHeight = 1.8f;  // Height of the character
	btScalar characterWidth = 0.5f;   // Width (radius) of the character
	btConvexShape* capsule = new btCapsuleShape(characterWidth, characterHeight);
	m_collisionShapes.push_back(capsule);

	ghostObject->setCollisionShape(capsule);
	ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	
	m_broadphase->getOverlappingPairCache()->setInternalGhostPairCallback((new btGhostPairCallback()));

	btScalar stepHeight = 0.35f;

	btKinematicCharacterController* character = new btKinematicCharacterController(
		ghostObject, capsule, stepHeight, btVector3(0.0f, 1.0f, 0.0f));

	m_dynamicsWorld->addAction(character);
	m_actions.push_back(character);
	character->warp(btVector3(0.0f, 5.0f, 0.0f));

	m_dynamicsWorld->addCollisionObject(ghostObject,
		btBroadphaseProxy::CharacterFilter,
		btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	m_collisionObjects.push_back(ghostObject);

	return character;
}

void PhysicsWorld::CreateStaticLevelGeometry(const std::string& pathToObj)
{
	btTriangleMesh* triangleMesh = Mesh::CreateCollisionMeshFromFile(pathToObj);
	m_mesh = triangleMesh; // see if this gets deleted

	bool useQuantizedAABBCompression = true;
	btBvhTriangleMeshShape* shape = new btBvhTriangleMeshShape(triangleMesh, useQuantizedAABBCompression);
	m_collisionShapes.push_back(shape);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(0, 0, 0)); // Position the mesh accordingly

	btScalar mass(0); // Mass of 0 for static objects
	btVector3 localInertia(0, 0, 0);
	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	m_motionStates.push_back(motionState);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	m_dynamicsWorld->addRigidBody(body);
	m_collisionObjects.push_back(body);
}

void PhysicsWorld::DebugDraw()
{
	m_dynamicsWorld->debugDrawWorld();
}

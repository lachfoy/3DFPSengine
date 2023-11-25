#include "PhysicsWorld.h"

#include <glm/gtc/type_ptr.hpp>
#include "Common.h"

#include "ResourceManager.h"
#include "Mesh.h"
#include "DebugRenderer.h"

CatCube::CatCube(const glm::vec3& position)
{
	m_mesh = Mesh::LoadMeshFromOBJ("data/models/cube.obj");

	m_texture = ResourceManager::Instance().GetTexture("cat");

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
	m_mesh = Mesh::LoadMeshFromOBJ("data/models/test.obj");

	m_texture = ResourceManager::Instance().GetTexture("missing");

	m_transform = glm::mat4(1.0f);
}

PhysicsWorld gPhysicsWorld;

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

	///// create a ground plane
	//btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	//m_collisionShapes.push_back(groundShape);

	//// Create a default motion state (position and rotation)
	//btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	//m_motionStates.push_back(groundMotionState);

	//// Create the rigid body construction info (mass = 0 => static body)
	//btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));

	//// Create the rigid body
	//btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

	//// Add the ground plane rigid body to the dynamics world
	//int groundCollisionGroup = btBroadphaseProxy::StaticFilter;
	//int groundCollisionMask = btBroadphaseProxy::AllFilter;
	//m_dynamicsWorld->addRigidBody(groundRigidBody, groundCollisionGroup, groundCollisionMask);
	//m_collisionObjects.push_back(groundRigidBody);
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


	SAFE_DELETE(m_mesh);
	SAFE_DELETE(m_ghostPairCallback);
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
	btRigidBody* body = new btRigidBody(rbInfo);

	// Add the body to the dynamics world
	m_dynamicsWorld->addRigidBody(body);
	m_collisionObjects.push_back(body);

	return catCube;
}

btKinematicCharacterController* PhysicsWorld::CreateCharacter(const glm::vec3& position)
{
	btPairCachingGhostObject* ghostObject = new btPairCachingGhostObject();

	btScalar characterHeight = 1.8f;  // Height of the character
	btScalar characterWidth = 0.5f;   // Width (radius) of the character
	btConvexShape* capsule = new btCapsuleShape(characterWidth, characterHeight);
	m_collisionShapes.push_back(capsule);

	ghostObject->setCollisionShape(capsule);
	ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	
	btGhostPairCallback* ghostPairCallback = new btGhostPairCallback();
	m_ghostPairCallback = ghostPairCallback;
	m_broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(ghostPairCallback);

	btScalar stepHeight = 0.35f;

	btKinematicCharacterController* character = new btKinematicCharacterController(
		ghostObject, capsule, stepHeight, btVector3(0.0f, 1.0f, 0.0f));

	m_dynamicsWorld->addAction(character);
	m_actions.push_back(character);

	m_dynamicsWorld->addCollisionObject(ghostObject,
		btBroadphaseProxy::CharacterFilter,
		btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	m_collisionObjects.push_back(ghostObject);

	btVector3 btPosition(position.x, position.y, position.z);
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btPosition);
	ghostObject->setWorldTransform(startTransform);

	return character;
}

void PhysicsWorld::CreateStaticLevelGeometry(const std::string& pathToObj)
{
	btTriangleMesh* triangleMesh = Mesh::LoadCollisionMeshFromOBJ(pathToObj);
	m_mesh = triangleMesh; // store reference so it can be deleted

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
	m_levelCollisionObject = body; // Save a reference to the body
}

bool PhysicsWorld::RayCast(const glm::vec3& from, const glm::vec3& direction)
{
	// Convert glm::vec3 to btVector3
	btVector3 btFrom(from.x, from.y, from.z);

	btVector3 btTo = btFrom + btVector3(direction.x, direction.y, direction.z) * 1000000.0f;

	// Perform raycast
	btCollisionWorld::ClosestRayResultCallback rayCallback(btFrom, btTo);
	m_dynamicsWorld->rayTest(btFrom, btTo, rayCallback);

	if (rayCallback.hasHit())
	{
		btVector3 btHit = rayCallback.m_hitPointWorld;
		glm::vec3 hit = glm::vec3(btHit.x(), btHit.y(), btHit.z());

		gDebugRenderer.AddLine(from, hit, glm::vec3(0.0f, 1.0f, 1.0f), 2.0f);
		gDebugRenderer.AddBox(hit, glm::vec3(0.1f), glm::vec3(0.0f, 1.0f, 1.0f), 2.0f);

		// Ray hit something
		return true;
	}

	return false;
}

bool PhysicsWorld::ExpensiveAABBTestVsLevelGeometry(const glm::vec3& position, const glm::vec3& halfExtents)
{
	btTransform worldTransform;
	worldTransform.setIdentity();
	btVector3 btOrigin = btVector3(position.x, position.y, position.z);
	worldTransform.setOrigin(btOrigin);

	btVector3 btHalfExtents = btVector3(halfExtents.x, halfExtents.y, halfExtents.z);
	btBoxShape aabbShape(btHalfExtents);

	btCollisionObject aabbObject;
	aabbObject.setCollisionShape(&aabbShape);
	aabbObject.setWorldTransform(worldTransform);

	MyContactCallback callback;
	m_dynamicsWorld->contactPairTest(&aabbObject, m_levelCollisionObject, callback);

	return callback.m_collisionDetected;
}

void PhysicsWorld::DebugDraw()
{
	m_dynamicsWorld->debugDrawWorld();
}

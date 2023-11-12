#include "PhysicsWorld.h"

#include <glm/gtc/type_ptr.hpp>



PhysicsWorld::PhysicsWorld()
{
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	m_overlappingPairCache = new btDbvtBroadphase();
	m_solver = new btSequentialImpulseConstraintSolver;
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);

	m_dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));  // Set gravity as needed

	m_debugDrawer = new BulletDebugDraw();
	m_dynamicsWorld->setDebugDrawer(m_debugDrawer);

	
	/// create a ground plane
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

	// Create a default motion state (position and rotation)
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

	// Create the rigid body construction info (mass = 0 => static body)
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));

	// Create the rigid body
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

	// Add the ground plane rigid body to the dynamics world
	int groundCollisionGroup = btBroadphaseProxy::StaticFilter;
	int groundCollisionMask = btBroadphaseProxy::AllFilter;
	m_dynamicsWorld->addRigidBody(groundRigidBody, groundCollisionGroup, groundCollisionMask);
}

PhysicsWorld::~PhysicsWorld()
{
	delete m_dynamicsWorld;
	delete m_solver;
	delete m_overlappingPairCache;
	delete m_dispatcher;
	delete m_collisionConfiguration;
	delete m_debugDrawer;
}

void PhysicsWorld::stepSimulation(float timeStep, int maxSubSteps)
{
	m_dynamicsWorld->stepSimulation(timeStep, maxSubSteps);
}

void PhysicsWorld::addRigidBody(btRigidBody* body)
{
	m_dynamicsWorld->addRigidBody(body);
}

void PhysicsWorld::removeRigidBody(btRigidBody* body)
{
	m_dynamicsWorld->removeRigidBody(body);
}

btRigidBody* PhysicsWorld::addBox(const glm::vec3& halfExtents, float mass, const glm::mat4& startTransform)
{
	btVector3 btHalfExtents(halfExtents.x, halfExtents.y, halfExtents.z);

	// Create the box shape
	btCollisionShape* boxShape = new btBoxShape(btHalfExtents);

	// Calculate the local inertia
	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f) {
		boxShape->calculateLocalInertia(mass, localInertia);
	}

	btTransform btStartTransform;
	btStartTransform.setFromOpenGLMatrix(glm::value_ptr(startTransform));

	// Create the rigid body's construction info
	btDefaultMotionState* motionState = new btDefaultMotionState(btStartTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, boxShape, localInertia);

	// Create the rigid body
	btRigidBody* body = new btRigidBody(rbInfo);

	// Add the body to the dynamics world
	m_dynamicsWorld->addRigidBody(body);

	return body;
}

void PhysicsWorld::CreateCharacter()
{
	btPairCachingGhostObject* ghostObject = new btPairCachingGhostObject();
	btScalar characterHeight = 1.8f;  // Height of the character
	btScalar characterWidth = 0.5f;   // Width (radius) of the character
	btConvexShape* capsule = new btCapsuleShape(characterWidth, characterHeight);

	ghostObject->setCollisionShape(capsule);
	ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	m_overlappingPairCache->getOverlappingPairCache()->setInternalGhostPairCallback((new btGhostPairCallback()));

	btScalar stepHeight = 0.35f;

	m_character = new btKinematicCharacterController(
		ghostObject, capsule, stepHeight, btVector3(0.0f, 1.0f, 0.0f));

	m_dynamicsWorld->addCollisionObject(ghostObject,
		btBroadphaseProxy::CharacterFilter,
		btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);

	m_dynamicsWorld->addAction(m_character);

	m_character->setWalkDirection(btVector3(0.0f, 0.0f, 0.0f));
	m_character->warp(btVector3(0.0f, 5.0f, 0.0f));
}

void PhysicsWorld::Render()
{
	m_dynamicsWorld->debugDrawWorld();

	btVector3 origin = m_character->getGhostObject()->getWorldTransform().getOrigin();
	printf("pos: %f,%f,%f\n", origin.x(), origin.y(), origin.z());
}

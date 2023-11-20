#include "NavGrid.h"

#include "DebugRenderer.h"
#include "PhysicsWorld.h"

void NavGrid::Generate(PhysicsWorld* physicsWorld)
{
	float gap = 2.0f;
	glm::vec3 nodeHalfExtents = glm::vec3(0.5f);

	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			glm::vec3 position = glm::vec3(x * gap, 1.0f, y * gap);
			
			bool collision = physicsWorld->ExpensiveAABBTestVsLevelGeometry(position, nodeHalfExtents);

			glm::vec3 color = collision ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::vec3(1.0f, 1.0f, 1.0f);

			gDebugRenderer.AddBox(position, nodeHalfExtents, color, 25.0f);
		}
	}
}

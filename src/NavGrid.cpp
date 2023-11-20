#include "NavGrid.h"

#include "DebugRenderer.h"

void NavGrid::Generate(PhysicsWorld* physicsWorld)
{
	float gap = 1.0f;
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			gDebugRenderer.AddSphere(glm::vec3(x * gap, 0.0f, y * gap), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		}
	}
}

#include "BulletDebugDraw.h"

#include "DebugRenderer.h"

#include <cstdio>

void BulletDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	glm::vec3 glmFrom = glm::vec3(from.x(), from.y(), from.z());
	glm::vec3 glmTo = glm::vec3(to.x(), to.y(), to.z());
	glm::vec3 glmColor = glm::vec3(color.x(), color.y(), color.z());

	gDebugRenderer.AddLine(glmFrom, glmTo, glmColor);
}

void BulletDebugDraw::reportErrorWarning(const char* warningString)
{
	printf("Bullet physics error: %s\n", warningString);
}

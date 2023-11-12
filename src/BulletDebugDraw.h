#pragma once

#include <bullet/btBulletDynamicsCommon.h>

class BulletDebugDraw : public btIDebugDraw
{
public:
	BulletDebugDraw() : m_debugMode(DBG_DrawWireframe) { }

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
	void reportErrorWarning(const char* warningString) override;

private:
	int m_debugMode;

};

#pragma once

#include <bullet/btBulletDynamicsCommon.h>

class BulletDebugDraw : public btIDebugDraw
{
public:
	BulletDebugDraw() : m_debugMode(DBG_DrawWireframe) { }

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
    
    void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB,
        btScalar distance, int lifeTime, const btVector3& color) override {
        // Implement drawing of contact points
    }

	void reportErrorWarning(const char* warningString) override;

    void draw3dText(const btVector3& location, const char* textString) override {
        // Implement how to draw text, if needed
    }

    void setDebugMode(int debugMode) override {
        m_debugMode = debugMode;
    }

    int getDebugMode() const override { return m_debugMode; }

private:
	int m_debugMode;

};

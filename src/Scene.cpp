#include "Scene.h"

#include "Renderer.h"
#include "DebugRenderer.h"

void Scene::Render()
{
	if (m_activeCamera)
	{
		gRenderer.Render(m_activeCamera);
		gDebugRenderer.Render(m_activeCamera);
	}
}

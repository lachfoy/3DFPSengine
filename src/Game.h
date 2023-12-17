#pragma once

#include <SDL.h>
#include <glad/glad.h>

#include "Scene.h"
#include <memory>

class GuiRenderer;

class Game
{
public:
	Game() {}

	bool Init(int windowedWidth, int windowedHeight, bool fullscreen);
	void Run();

private:
	void SetupGL();
	void Cleanup();

	SDL_Window* m_window;
	SDL_GLContext m_context;

	//bool m_quitRequested = false;

	int m_windowWidth;
	int m_windowHeight;

	GuiRenderer* m_guiRenderer;
	
	std::unique_ptr<Scene> m_scene;

};

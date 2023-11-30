#pragma once

#include <SDL.h>
#include <glad/glad.h>
#include <SDL_opengl.h>

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
	int m_windowWidth;
	int m_windowHeight;
	int m_viewportWidth;
	int m_viewportHeight;

	GuiRenderer* m_guiRenderer;

};

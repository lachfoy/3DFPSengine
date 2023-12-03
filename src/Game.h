#pragma once

#include <SDL.h>
#include <glad/glad.h>

class GuiRenderer;

class Game
{
public:
	Game() {}
	bool Init(int windowedWidth, int windowedHeight, bool fullscreen);
	void Run();

private:
	void Cleanup();

	SDL_Window* m_window;
	GuiRenderer* m_guiRenderer;

};

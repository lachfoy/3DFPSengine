#pragma once

#include "NonCopyable.h"
#include <SDL.h>
#include <glad/glad.h>
#include <SDL_opengl.h>
#include <string>

struct WindowConfig
{
	int windowedWidth = 1280;
	int windowedHeight = 720;
	bool startInFullscreen = false;
};

class Window : public NonCopyable
{
public:
	void Init(const WindowConfig& config);

	void PollEvents();
	bool QuitRequested() const { return m_quitRequested; }
	void WarpMouseInWindow();

	SDL_Window* GetWindow() { return m_window; } // Very temp.... get rid of this whole class please
	
	void SetTitle(const char* title);
	void SetVsyncEnabled(bool enabled);
	void ToggleFullscreen();
	void SwapBuffers();
	void Quit();

	void Destroy();

private:
	void SetupGL();

	SDL_Window* m_window;
	SDL_GLContext m_context;
	bool m_quitRequested = false;
	int m_windowWidth;
	int m_windowHeight;
	int m_windowedWidth;
	int m_windowedHeight;
	bool m_hasFocus = false;
	bool m_isFullscreen = false;

};

extern Window gWindow;

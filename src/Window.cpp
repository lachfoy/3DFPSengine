#include "Window.h"

#include "Input.h"

Window gWindow;

void Window::Init(const WindowConfig& config)
{
	Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

	// save whatever the windowed width and height should be
	// even if we start in fullscreen
	m_windowedWidth = config.windowedWidth;
	m_windowedHeight = config.windowedHeight;

	if (config.startInFullscreen)
	{
		SDL_DisplayMode displayMode;
		SDL_GetDesktopDisplayMode(0, &displayMode); // can return int error
		m_windowWidth = displayMode.w;
		m_windowHeight = displayMode.h;

		windowFlags |= SDL_WINDOW_BORDERLESS;
		m_isFullscreen = true;
	}
	else
	{
		m_windowWidth = m_windowedWidth;
		m_windowHeight = m_windowedHeight;
		m_isFullscreen = false;
	}

	m_window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_windowWidth, m_windowHeight, windowFlags);
	if (!m_window)
	{
		printf("An error occurred while creating an SDL2 window: %s\n", SDL_GetError());
		SDL_Quit();
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	m_context = SDL_GL_CreateContext(m_window);
	if (!m_context)
	{
		printf("GL Context could not be created: %s\n", SDL_GetError());
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		printf("GLAD could not be loaded\n");
		SDL_GL_DeleteContext(m_context);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}

	// Hide the cursor. I don't think we wanna expose this to the user
	SDL_ShowCursor(SDL_DISABLE);

	SetupGL();
}

void Window::PollEvents()
{
	// We pass input events over to the input class here
	// It might be better to have our own global event system that the input class can subscribe to instead?

	SDL_Event event;
	// handle input events
	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
		case SDL_QUIT:
			m_quitRequested = true;
		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				SDL_SetWindowGrab(m_window, SDL_TRUE);
				m_hasFocus = true;
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				SDL_SetWindowGrab(m_window, SDL_FALSE);
				m_hasFocus = false;
				break;
			}
		}
	}
}

void Window::WarpMouseInWindow()
{
	// Warp the mouse position to the center of the window
	if (m_hasFocus)
	{
		SDL_WarpMouseInWindow(m_window, m_windowWidth / 2, m_windowHeight / 2);
	}
}

void Window::SetTitle(const char* title)
{
	SDL_SetWindowTitle(m_window, title);
}

void Window::SetVsyncEnabled(bool enabled)
{
	int vsync = enabled ? 1 : 0;
	if (SDL_GL_SetSwapInterval(vsync) != 0)
	{
		printf("Error setting Vsync\n");
	}
}

void Window::ToggleFullscreen()
{
	m_isFullscreen = !m_isFullscreen;

	if (m_isFullscreen)
	{
		SDL_DisplayMode displayMode;
		SDL_GetDesktopDisplayMode(0, &displayMode); // can return int error
		m_windowWidth = displayMode.w;
		m_windowHeight = displayMode.h;
		SDL_SetWindowSize(m_window, m_windowWidth, m_windowHeight);

		glViewport(0, 0, m_windowWidth, m_windowHeight);

		SDL_SetWindowBordered(m_window, SDL_FALSE);
	}
	else
	{
		m_windowWidth = m_windowedWidth;
		m_windowHeight = m_windowedHeight;
		SDL_SetWindowSize(m_window, m_windowWidth, m_windowHeight);

		glViewport(0, 0, m_windowWidth, m_windowHeight);

		SDL_SetWindowBordered(m_window, SDL_TRUE);
	}
}

void Window::SwapBuffers()
{
	SDL_GL_SwapWindow(m_window);
}

void Window::Quit()
{
	m_quitRequested = true;
}

void Window::Destroy()
{
	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);

	SDL_Quit();
}

void Window::SetupGL()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
}

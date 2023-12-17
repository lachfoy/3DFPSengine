#pragma once

#include <SDL.h>
#include <glm/glm.hpp>

class Input
{
friend class Window;
public:
	Input(SDL_Window* window);
	~Input();

	void Update();

	Uint8 GetCurrentKeyState(Uint8 key);
	bool KeyPressed(Uint8 key);
	bool KeyReleased(Uint8 key);

	glm::vec2 GetMouseAbsPos();
	glm::vec2 GetMouseRelPos();

	bool LMBDown() { return m_lmbDown; }
	bool RMBDown() { return m_rmbDown; }

	bool LMBClicked() { return m_lmbClicked; }
	bool RMBClicked() { return m_rmbClicked; }

	bool QuitRequested() { return m_quit; }

private:
	int m_numKeys;
	Uint8 *m_keyboardState;
	Uint8 *m_lastKeyboardState;
	size_t m_keyboardStateSize;

	// mouse
	glm::vec2 m_mouseAbsPos;
	glm::vec2 m_mouseRelPos;

	bool m_lmbDown;
	bool m_rmbDown;
	bool m_lmbClicked;
	bool m_rmbClicked;

	SDL_Window* m_window;
	bool m_quit;

};

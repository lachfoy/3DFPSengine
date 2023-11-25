#pragma once

#include <SDL.h>
#include <glm/glm.hpp>

#define NUM_MOUSE_BUTTONS 5

class Input
{
public:
	static Input& Instance()
	{
		static Input instance;
		return instance;
	}

	bool HandleEvents();
	void Update();

	bool IsKeyPressed(Uint8 key) const;
	bool IsKeyHeld(Uint8 key) const;
	glm::vec2 GetMouseAbsPos() const;
	glm::vec2 GetMouseRelPos();

	bool IsMouseButtonPressed(Uint8 button) const;
	bool IsMouseButtonHeld(Uint8 button) const;

private:
	Input();
	~Input();

	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

	SDL_Event m_event;
	int m_numKeys;
	bool *m_keyboardState;
	bool *m_lastKeyboardState;

	// mouse
	glm::vec2 m_mouseAbsPos;
	glm::vec2 m_mouseRelPos;

	// 5 mouse buttons
	//SDL_BUTTON_LEFT
	//SDL_BUTTON_MIDDLE
	//SDL_BUTTON_RIGHT
	//SDL_BUTTON_X1
	//SDL_BUTTON_X2
	bool m_mouseState[NUM_MOUSE_BUTTONS];
	bool m_lastMouseState[NUM_MOUSE_BUTTONS];

};

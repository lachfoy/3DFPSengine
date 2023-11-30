#include "Input.h"

Input gInput;

Input::Input()
{
	// find the number of keys
	SDL_GetKeyboardState(&m_numKeys);
	m_keyboardState = new bool[m_numKeys];
	m_lastKeyboardState = new bool[m_numKeys];

	// intialize the states as false
	for (int i = 0; i < m_numKeys; i++) {
		m_keyboardState[i] = false;
		m_lastKeyboardState[i] = false;
	}
}

Input::~Input()
{
	// delete arrays
	delete[] m_keyboardState;
	delete[] m_lastKeyboardState;
}

void Input::Update()
{
	// update last state
	for (int i = 0; i < m_numKeys; i++)
	{
		m_lastKeyboardState[i] = m_keyboardState[i];
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; i++)
	{
		m_lastMouseState[i] = m_mouseState[i];
	}
}

bool Input::IsKeyPressed(Uint8 key) const
{
	return m_keyboardState[key] && !m_lastKeyboardState[key];
}

bool Input::IsKeyHeld(Uint8 key) const
{
	return m_keyboardState[key];
}

glm::vec2 Input::GetMouseAbsPos() const
{
	return m_mouseAbsPos;
}

glm::vec2 Input::GetMouseRelPos()
{
	glm::vec2 pos = m_mouseRelPos;
	m_mouseRelPos = glm::vec2(0.0f);
	return pos;
}

bool Input::IsMouseButtonPressed(Uint8 button) const
{
	return m_mouseState[button] && !m_lastMouseState[button];
}

bool Input::IsMouseButtonHeld(Uint8 button) const
{
	return m_mouseState[button];
}

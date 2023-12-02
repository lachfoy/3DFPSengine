#include "Input.h"

Input::Input(SDL_Window* window) : m_window(window)
{
	// Find the number of keys
	SDL_GetKeyboardState(&m_numKeys);
	m_keyboardState = new Uint8[m_numKeys];
	m_lastKeyboardState = new Uint8[m_numKeys];

	m_keyboardStateSize = m_numKeys * sizeof(Uint8);

	// Intialize the states as 0
	for (int i = 0; i < m_numKeys; i++)
	{
		m_keyboardState[i] = 0;
		m_lastKeyboardState[i] = 0;
	}
}

Input::~Input()
{
	// Delete arrays
	delete[] m_keyboardState;
	delete[] m_lastKeyboardState;
}

void Input::Update()
{
	SDL_Event sdlEvent;

	m_lmbClicked = false;
	m_rmbClicked = false;

	while (SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type)
		{
		case SDL_MOUSEMOTION:
			m_mouseAbsPos = glm::vec2(static_cast<float>(sdlEvent.motion.x), static_cast<float>(sdlEvent.motion.y));
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (sdlEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (!m_lmbDown)
					m_lmbClicked = true;

				m_lmbDown = true;
			}
			else if (sdlEvent.button.button == SDL_BUTTON_RIGHT)
			{
				if (!m_rmbDown)
					m_rmbClicked = true;

				m_rmbDown = true;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (sdlEvent.button.button == SDL_BUTTON_LEFT)
				m_lmbDown = false;
			else if (sdlEvent.button.button == SDL_BUTTON_RIGHT)
				m_rmbDown = false;
			break;
		}
	}

	int relX, relY;
	SDL_GetRelativeMouseState(&relX, &relY);
	m_mouseRelPos = glm::vec2(static_cast<float>(relX), static_cast<float>(relY));

	// Copy states to past state buffer
	memcpy(m_lastKeyboardState, m_keyboardState, m_keyboardStateSize);

	// Copy new states
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	memcpy(m_keyboardState, keyboardState, m_keyboardStateSize);
}

Uint8 Input::GetCurrentKeyState(Uint8 key)
{
	return m_keyboardState[key];
}

bool Input::KeyPressed(Uint8 key)
{
	return !m_lastKeyboardState[key] && m_keyboardState[key];
}

bool Input::KeyReleased(Uint8 key)
{
	return m_lastKeyboardState[key] && !m_keyboardState[key];
}

glm::vec2 Input::GetMouseAbsPos()
{
	return m_mouseAbsPos;
}

glm::vec2 Input::GetMouseRelPos()
{
	glm::vec2 pos = m_mouseRelPos;
	m_mouseRelPos = glm::vec2(0.0f);
	return pos;
}

#include "Button.h"

#include "Input.h"

Button::Button(const char* name, GuiRenderer* guiRenderer) : Panel(name, guiRenderer)
{
	m_color = glm::vec3(0.6, 0.6, 0.6);
}

Button::Button(const char* name, GuiRenderer* guiRenderer, glm::vec2 position, glm::vec2 size) : Panel(name, guiRenderer, position, size)
{
	m_color = glm::vec3(0.6, 0.6, 0.6);
}

Button::Button(const char* name, GuiRenderer* guiRenderer, glm::vec2 position, glm::vec2 size, Panel* parent) : Panel(name, guiRenderer, position, size, parent)
{
	m_color = glm::vec3(0.6, 0.6, 0.6);
}

bool Button::HandleInput(Input* input)
{
	if (!m_enabled) return false;

	m_buttonState = ButtonState::NORMAL;
	SetScale(1.0f);

	glm::vec2 mousePos = input->GetMouseAbsPos() / 2.0f; // Hardcoded scaling factor. although the scale factor should be different for the UI
	
	if (IsInBounds(mousePos))
	{
		m_buttonState = ButtonState::HOVERED;
		SetScale(1.05f);

		if (input->IsMouseButtonPressed(SDL_BUTTON_LEFT))
		{
			if (m_callback)
			{
				m_callback();
			}
			else
			{
				printf("%s has no assigned callback function\n", m_name.c_str());
			}

			return true;
		}

		if (input->IsMouseButtonHeld(SDL_BUTTON_LEFT))
		{
			m_buttonState = ButtonState::PRESSED;
		}
	}

	return false;
}

void Button::Render()
{
	switch (m_buttonState)
	{
	case ButtonState::NORMAL:
		m_color = glm::vec3(0.6, 0.6, 0.6);
		m_alpha = 1.0f;

		break;
	case ButtonState::HOVERED:
		m_color = glm::vec3(0.7, 0.7, 0.7);
		m_alpha = 1.0f;

		break;
	case ButtonState::PRESSED:
		m_color = glm::vec3(0.55, 0.55, 0.55);
		m_alpha = 1.0f;

		break;
	case ButtonState::DISABLED:
		m_color = glm::vec3(0.6, 0.6, 0.6);
		m_alpha = 0.3f;
		break;
	default:
		break;
	}

	Panel::Render();
}

void Button::SetEnabled(bool enabled)
{
	m_enabled = enabled;

	if (!m_enabled)
	{
		m_buttonState = ButtonState::DISABLED;
	}
}

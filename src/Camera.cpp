#include "Camera.h"

#include <cmath>
#include "Input.h"

Camera::Camera(glm::vec3 position, glm::vec3 up)
	: m_position(position), m_worldUp(up)
{
	UpdateCameraVectors();
}

void Camera::HandleInput(Input &input)
{
	if (input.IsKeyHeld(SDL_SCANCODE_W))
	{
		m_position += m_speed * m_front;
	}
	if (input.IsKeyHeld(SDL_SCANCODE_A))
	{
		m_position -= m_speed * m_right;
	}
	if (input.IsKeyHeld(SDL_SCANCODE_S))
	{
		m_position -= m_speed * m_front;
	}
	if (input.IsKeyHeld(SDL_SCANCODE_D))
	{
		m_position += m_speed * m_right;
	}

	// Mouse movement
	glm::vec2 mouseRelPos = input.GetMouseRelPos();
	float xOffset = mouseRelPos.x * m_mouseSensitivity;
	float yOffset = -mouseRelPos.y * m_mouseSensitivity;
	
	m_yaw += xOffset;
	m_pitch += yOffset;
	
	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (m_pitch > 89.0f)
	{
		m_pitch = 89.0f;
	}
	
	if (m_pitch < -89.0f)
	{
		m_pitch = -89.0f;
	}

	UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);

	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

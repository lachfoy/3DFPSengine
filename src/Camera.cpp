#include "Camera.h"

#include "Global.h"
#include <cmath>
#include "Input.h"

#include <string>

#include "TextRenderer.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& up)
	: m_position(position), m_worldUp(up)
{
	UpdateCameraVectors();
}

const glm::mat4 Camera::GetProjection()
{
	if (m_projectionNeedsUpdating)
	{
		m_projection = glm::perspective(glm::radians(m_fov), m_aspect, m_nearClip, m_farClip);
		m_projectionNeedsUpdating = false;
	}

	return m_projection;
}

void Camera::SetFov(float fov)
{
	m_fov = fov;
	m_projectionNeedsUpdating = true;
}

void Camera::SetAspect(float aspect)
{
	m_aspect = aspect;
	m_projectionNeedsUpdating = true;
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

void FirstPersonCamera::Update(float dt)
{
	// Mouse movement
	glm::vec2 mouseRelPos = global.input->GetMouseRelPos();
	float xOffset = mouseRelPos.x * m_mouseSensitivity;
	float yOffset = -mouseRelPos.y * m_mouseSensitivity;

	m_yaw += xOffset;
	m_pitch += yOffset;

	if (m_pitch > 70.0f)
	{
		m_pitch = 70.0f;
	}

	if (m_pitch < -70.0f)
	{
		m_pitch = -70.0f;
	}

	UpdateCameraVectors();
}

void FlyingCamera::Update(float dt)
{
	m_moveDir = glm::vec3(0.0f);

	//if (global.input->IsKeyHeld(SDL_SCANCODE_W))
	//{
	//	m_moveDir += m_front;
	//}
	//if (global.input->IsKeyHeld(SDL_SCANCODE_A))
	//{
	//	m_moveDir -= m_right;
	//}
	//if (global.input->IsKeyHeld(SDL_SCANCODE_S))
	//{
	//	m_moveDir -= m_front;
	//}
	//if (global.input->IsKeyHeld(SDL_SCANCODE_D))
	//{
	//	m_moveDir += m_right;
	//}

	// Mouse movement
	glm::vec2 mouseRelPos = global.input->GetMouseRelPos();
	float xOffset = mouseRelPos.x * m_mouseSensitivity;
	float yOffset = -mouseRelPos.y * m_mouseSensitivity;

	m_yaw += xOffset;
	m_pitch += yOffset;

	m_position += m_moveDir * m_speed * dt;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (m_pitch > 70.0f)
	{
		m_pitch = 70.0f;
	}

	if (m_pitch < -70.0f)
	{
		m_pitch = -70.0f;
	}

	UpdateCameraVectors();

	std::string debugString;
	debugString += "pos:" + std::to_string(m_position.x) + ", " + std::to_string(m_position.y) + ", " + std::to_string(m_position.z) + "\n";
	debugString += "pitch:" + std::to_string(m_pitch) + "\n";
	debugString += "yaw:" + std::to_string(m_yaw) + "\n";
	gTextRenderer.AddStringToBatch(debugString, 0, 0, glm::vec3(1.0f));
}
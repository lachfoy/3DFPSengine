#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
friend class Renderer;
friend class DebugRenderer;
public:
	Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));
	~Camera() {}

	const glm::mat4 GetView() const { return glm::lookAt(m_position, m_position + m_front, m_worldUp); }
	const glm::mat4 GetProjection();

	void SetFov(float fov);
	void SetAspect(float aspect);

	virtual void Update(float dt) {}

	// very temp getters, delete these
	const glm::vec3 GetPosition() const { return m_position; }
	const glm::vec3 GetFront() const { return m_front; }
	const glm::vec3 GetRight() const { return m_right; }

	void SetPosition(const glm::vec3& position) { m_position = position; }

protected:
	void UpdateCameraVectors();
	
	glm::vec3 m_position;
	glm::vec3 m_worldUp;
	glm::vec3 m_right;
	glm::vec3 m_up;
	float m_yaw = -90.0f;
	float m_pitch = 0.0f;
	float m_mouseSensitivity = 0.25f;
	float m_fov = 90.0f; // y fov
	glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);

	float m_nearClip = 0.1f;
	float m_farClip = 100.0f;

	glm::mat4 m_projection;
	float m_aspect;
	bool m_projectionNeedsUpdating = false;

};

class FirstPersonCamera : public Camera
{
public:
	virtual void Update(float dt);
};

class FlyingCamera: public Camera
{
public:
	virtual void Update(float dt);

private:
	float m_speed = 6.0f;
	glm::vec3 m_moveDir;

};


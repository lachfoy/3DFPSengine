#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Input;
class Camera
{
friend class Renderer;
public:
	Camera() = default;
	Camera(glm::vec3 position, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
	~Camera() {}

	glm::mat4 GetView() const { return glm::lookAt(m_position, m_position + m_front, m_worldUp); }

	void HandleInput(Input* input);
	void Update(float dt);

private:
	void UpdateCameraVectors();
	
	glm::vec3 m_position;
	glm::vec3 m_worldUp;
	glm::vec3 m_right;
	glm::vec3 m_up;
	float m_yaw = -90.0f;
	float m_pitch = 0.0f;
	float m_speed = 6.00f; //6.0f;
	float m_mouseSensitivity = 0.25f;
	float m_fov = 45.0f;
	glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);

	glm::vec3 m_moveDir;

};

#endif
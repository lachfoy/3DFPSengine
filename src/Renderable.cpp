#include "Renderable.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "Renderer.h"
//m_scale(glm::vec3(1.0f, 1.0f, 1.0f)), m_rotation(glm::vec3(0.0f, 0.0f, 0.0f))

Renderable::Renderable() : m_scale(glm::vec3(1.0f, 1.0f, 1.0f)), m_rotation(glm::vec3(0.0f, 0.0f, 0.0f))
{

}

void Renderable::UpdateTransform()
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_position);
	glm::mat4 rotationMatrix = glm::eulerAngleYXZ(m_rotation.y, m_rotation.x, m_rotation.z); //glm::toMat4(rotation);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);

	// Combine the matrices
	m_transform = translationMatrix * rotationMatrix * scaleMatrix;
}

void Renderable::Render()
{
	gRenderer.AddToRenderList(this);
}

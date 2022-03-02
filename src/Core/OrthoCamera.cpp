#include "Core/OrthoCamera.h"
#include <glm/gtc/matrix_transform.hpp>

OrthoCamera::OrthoCamera()
{
	m_position = {0.0f, 0.0f};
	m_projection = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f);
}

void OrthoCamera::SetSize(int width, int height)
{
    m_projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
}

void OrthoCamera::SetPosition(const glm::vec2& position)
{
	m_position = position;
}

const glm::mat4 OrthoCamera::GetView() const
{
	return glm::translate(glm::mat4(1.0f), -glm::vec3(m_position, 0.0f));
}
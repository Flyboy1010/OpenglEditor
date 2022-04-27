#include "Core/EditorCamera.h"
#include "Core/Input.h"
#include <glm/gtc/matrix_transform.hpp>

EditorCamera::EditorCamera()
{
	m_position = { 0.0f, 0.0f, 0.0f };
	m_forward = { 0.0f, 0.0f, -1.0f };
	m_right = { 1.0f, 0.0f, 0.0f };
	m_up = { 0.0f, 1.0f, 0.0f };
	m_focalPoint = { 0.0f, 0.0f, 0.0f };
	m_transform = glm::mat4(1.0f);
	m_yaw = 45.0f, m_pitch = 45.0f;
	m_fov = 70.0f;
	m_znear = 0.01f, m_zfar = 1000.0f;
	m_aspectRatio = 16.0f / 9.0f;
	m_distance = 5.0f;
	m_projection = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_znear, m_zfar);

	m_viewportWidth = 1280, m_viewportHeight = 720;

	m_lastMousePosition = { 0.0f, 0.0f };
	m_using = false;
}

void EditorCamera::SetViewport(float width, float height)
{
	m_viewportWidth = width;
	m_viewportHeight = height;

	UpdateProjection();
}

void EditorCamera::UpdateProjection()
{
	m_aspectRatio = m_viewportWidth / m_viewportHeight;
	m_projection = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_znear, m_zfar);
}

void EditorCamera::Update(float delta)
{
	// calculate the position

	if (Input::KeyPressed(GLFW_KEY_LEFT_ALT))
	{
		glm::vec2 mouse = { Input::GetMouseX(), Input::GetMouseY() };
		glm::vec2 delta = mouse - m_lastMousePosition;
		m_lastMousePosition = mouse;
		float mouseScrollDelta = Input::GetMouseScrollDelta();
		m_using = true;

		if (Input::MouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			/* MOUSE ROTATION */

			const float sensitivity = 0.175f;
			m_yaw += delta.x * sensitivity;
			m_pitch -= delta.y * sensitivity;

			if (m_pitch < 1.0f) m_pitch = 1.0f;
			else if (m_pitch > 179.0f) m_pitch = 179.0f;
		}
		else if (Input::MouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
		{
			/* MOUSE PANNING */

			const float sensitivity = 0.01f;

			m_focalPoint += (delta.x * m_right - delta.y * m_up) * sensitivity;
		}
		else if (mouseScrollDelta != 0)
		{
			/* ZOOMING */

			const float sensitivity = 1.0f;

			m_distance -= mouseScrollDelta * sensitivity;

			if (m_distance < 0.1f)
				m_distance = 0.1f;
		}
	}
	else
	{
		// the camera is not moving

		m_using = false;
	}

	/* CALCULATE VECTORS */

	const glm::vec3 worldUp = { 0.0f, 1.0f, 0.0f };

	// spherical coordinates baby

	m_forward.x = glm::sin(glm::radians(m_pitch)) * glm::cos(glm::radians(m_yaw));
	m_forward.y = glm::cos(glm::radians(m_pitch));
	m_forward.z = glm::sin(glm::radians(m_pitch)) * glm::sin(glm::radians(m_yaw));
	m_forward = glm::normalize(m_forward);

	m_right = glm::normalize(glm::cross(m_forward, worldUp));

	m_up = glm::normalize(glm::cross(m_forward, m_right));

	/* CALCULATE POSITION */

	m_position = m_distance * m_forward + m_focalPoint; // translate to the focal point

	/* CALCULATE TRANSFORM */

	m_transform = glm::lookAt(m_position, m_focalPoint, worldUp);
}

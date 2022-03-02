#include "Core/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Core/Input.h"

Camera::Camera()
{
	m_projection = glm::perspective(glm::radians(70.0f), 16.0f / 9.0f, 0.01f, 1000.0f);
	m_position = { 5.0f, 5.0f, 5.0f };
	m_transform = glm::lookAt(m_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_yaw = 90;
	m_pitch = 90;
	m_offsetX = 0;
	m_offsetY = 0;
}

void Camera::SetProjection(float fov, float ar, float znear, float zfar)
{
	m_projection = glm::perspective(glm::radians(fov), ar, znear, zfar);
}

void Camera::Update(float delta)
{
	if (Input::MouseButtonJustPressed(GLFW_MOUSE_BUTTON_RIGHT))
	{
		Input::SetMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		float mouseX, mouseY;
		Input::GetMousePos(&mouseX, &mouseY);
		m_offsetX = mouseX;
		m_offsetY = mouseY;
	}
	
	if (Input::MouseButtonJustReleased((GLFW_MOUSE_BUTTON_RIGHT)))
	{
		Input::SetMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (Input::MouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
	{
		// get the mouse position

		float mouseX, mouseY;
		Input::GetMousePos(&mouseX, &mouseY);

		// yaw and pitch

		const float sensitivity = 0.075f;

		m_yaw += (mouseX - m_offsetX) * sensitivity;
		m_pitch += (mouseY - m_offsetY) * sensitivity;

		if (m_pitch < 1.0f) m_pitch = 1.0f;
		else if (m_pitch > 179.0f) m_pitch = 179.0f;

		m_offsetX = mouseX;
		m_offsetY = mouseY;

		// calculate vectors

		const glm::vec3 up = { 0.0f, 1.0f, 0.0f };

		glm::vec3 forward;
		forward.x = glm::sin(glm::radians(m_pitch)) * glm::cos(glm::radians(m_yaw));
		forward.y = glm::cos(glm::radians(m_pitch));
		forward.z = glm::sin(glm::radians(m_pitch)) * glm::sin(glm::radians(m_yaw));
		forward = glm::normalize(forward);

		glm::vec3 right = glm::normalize(glm::cross(forward, up));

		// camera movement

		const float moveSpeed = 5.0f;

		if (Input::KeyPressed(GLFW_KEY_W))
			m_position += forward * moveSpeed * delta;
		else if (Input::KeyPressed(GLFW_KEY_S))
			m_position -= forward * moveSpeed * delta;
		if (Input::KeyPressed(GLFW_KEY_D))
			m_position += right * moveSpeed * delta;
		else if (Input::KeyPressed(GLFW_KEY_A))
			m_position -= right * moveSpeed * delta;
		if (Input::KeyPressed(GLFW_KEY_Q))
			m_position += up * moveSpeed * delta;
		else if (Input::KeyPressed(GLFW_KEY_E))
			m_position -= up * moveSpeed * delta;

		// transform matrix

		m_transform = glm::lookAt(m_position, m_position + forward, up);
	}
}

#pragma once

#include <glm/glm.hpp>

class EditorCamera
{
public:
	EditorCamera();

	const glm::vec3& GetPosition() const { return m_position; }
	const glm::mat4& GetTransform() const { return m_transform; }
	const glm::mat4& GetProjection() const { return m_projection; }
	bool IsUsing() const { return m_using; }

	void SetViewport(float width, float height);

	void Update(float delta);

private:
	void UpdateProjection();

private:
	glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec3 m_right;
	glm::vec3 m_up;
	glm::vec3 m_focalPoint;
	glm::mat4 m_transform;
	glm::mat4 m_projection;
	float m_yaw, m_pitch;
	float m_fov;
	float m_znear, m_zfar;
	float m_aspectRatio;
	float m_distance;

	float m_viewportWidth, m_viewportHeight;

	glm::vec2 m_lastMousePosition;
	bool m_using;
};
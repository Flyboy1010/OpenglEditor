#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera();

	const glm::vec3& GetPosition() const { return m_position; }
	const glm::mat4& GetProjection() const { return m_projection; }
	const glm::mat4& GetTransform() const { return m_transform; }

	void SetProjection(float fov, float ar, float znear, float zfar);

	void Update(float delta);

private:
	glm::vec3 m_position;
	glm::mat4 m_projection;
	glm::mat4 m_transform;
	float m_yaw, m_pitch;
	float m_offsetX, m_offsetY;
};
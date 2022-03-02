#pragma once

#include <glm/glm.hpp>

class OrthoCamera
{
public:
	OrthoCamera();

	void SetSize(int width, int height);

	void SetPosition(const glm::vec2& position);
	const glm::vec2& GetPosition() const { return m_position; }
	const glm::mat4& GetProjection() const { return m_projection; }
	const glm::mat4 GetView() const;

private:
	glm::vec2 m_position;
	glm::mat4 m_projection;
};
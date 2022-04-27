#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <unordered_map>

class Shader
{
public:
	Shader();
	Shader(const Shader&) = delete;
	Shader(Shader&& other) noexcept;
	Shader(const std::string& path);
	~Shader();

	unsigned int GetId() const { return m_id; }

	void Load(const std::string& path);

	void Bind() const;
	void UnBind() const;

	void SetUniformMat4(const std::string& name, const glm::mat4& mat);
	void SetUniform1iv(const std::string& name, int count, const int* data);
	void SetUniform1i(const std::string& name, int data);
	void SetUniform1f(const std::string& name, float data);
	void SetUniformVec2(const std::string& name, const glm::vec2& data);
	void SetUniformVec3(const std::string& name, const glm::vec3& data);
	void SetUniformVec4(const std::string& name, const glm::vec4& data);
	void SetUniformVec2v(const std::string& name, int count, const glm::vec2* data);
	void SetUniformVec3v(const std::string& name, int count, const glm::vec3* data);

	// delete assign operator

	Shader& operator=(const Shader&) = delete;

private:
	int GetUniformLocation(const std::string& name);

private:
	unsigned int m_id;
	std::string m_path;
	std::unordered_map<std::string, int> m_uniformCache;
};
#pragma once

#include <iostream>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader();
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	Shader(const Shader&) = delete;
	~Shader();

	unsigned int GetId() const { return m_id; }

	void Load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

	void Bind() const;
	void UnBind() const;

	void SetUniformMat4(const std::string& name, const glm::mat4& mat) const;
	void SetUniform1iv(const std::string& name, int count, const int* data) const;
	void SetUniform1i(const std::string& name, int data) const;
	void SetUniform1f(const std::string& name, float data) const;
	void SetUniformVec2(const std::string& name, const glm::vec2& data) const;
	void SetUniformVec3(const std::string& name, const glm::vec3& data) const;
	void SetUniformVec4(const std::string& name, const glm::vec4& data) const;
	void SetUniformVec2v(const std::string& name, int count, const glm::vec2* data) const;
	void SetUniformVec3v(const std::string& name, int count, const glm::vec3* data) const;

	// delete assign operator

	Shader& operator=(const Shader&) = delete;

private:
	std::string Parse(const std::string& path);
	unsigned int Compile(const std::string& code, int type);

private:
	unsigned int m_id;
	std::string m_vertexShaderPath;
	std::string m_fragmentShaderPath;
};
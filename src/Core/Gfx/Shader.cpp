#include "Core/Gfx/Shader.h"
#include <GL/glew.h>
#include "Core/Logger.h"
#include <sstream>
#include <fstream>
#include <string>

Shader::Shader()
{
	m_id = 0;
	m_vertexShaderPath = "";
	m_fragmentShaderPath = "";
}

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	Load(vertexShaderPath, fragmentShaderPath);
}

Shader::~Shader()
{
	glDeleteProgram(m_id);

	LOG_INFO("Shader destroyed \"%s\" \"%s\"", m_vertexShaderPath.c_str(), m_fragmentShaderPath.c_str());
}

std::string Shader::Parse(const std::string& path)
{
	std::ifstream shaderFile(path);

	std::stringstream shaderCode;
	std::string line;

	if (shaderFile.is_open())
	{
		while (std::getline(shaderFile, line))
		{
			shaderCode << line << "\n";
		}
	}

	return shaderCode.str();
}

unsigned int Shader::Compile(const std::string& code, int type)
{
	unsigned int shaderId = glCreateShader(type);

	const char* cstr_code = code.c_str();

	glShaderSource(shaderId, 1, &cstr_code, nullptr);
	glCompileShader(shaderId);

	int compileStatus;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus == GL_FALSE)
	{
		int length;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(shaderId, length, &length, message);

		// parse error (remove '\n')

		for (int i = 0; i < strnlen(message, length); i++)
		{
			if (message[i] == '\n')
				message[i] = ' ';
		}

		LOG_ERROR("%s", message);

		delete[] message;

		glDeleteShader(shaderId);

		return 0;
	}

	return shaderId;
}

void Shader::Load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	m_vertexShaderPath = vertexShaderPath;
	m_fragmentShaderPath = fragmentShaderPath;

	std::string vertexShaderCode = Parse(vertexShaderPath);
	std::string fragmentShaderCode = Parse(fragmentShaderPath);

	unsigned int vertexShaderId = Compile(vertexShaderCode, GL_VERTEX_SHADER);
	unsigned int fragmentShaderId = Compile(fragmentShaderCode, GL_FRAGMENT_SHADER);

	if (vertexShaderId != 0 && fragmentShaderId != 0)
	{
		m_id = glCreateProgram();

		glAttachShader(m_id, vertexShaderId);
		glAttachShader(m_id, fragmentShaderId);
		glLinkProgram(m_id);
		glValidateProgram(m_id);

		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);

		LOG_INFO("Shader loaded \"%s\", \"%s\"", m_vertexShaderPath.c_str(), m_fragmentShaderPath.c_str());
	}
	else
	{
		m_id = 0;
		LOG_ERROR("Shader loading \"%s\", \"%s\"", m_vertexShaderPath.c_str(), m_fragmentShaderPath.c_str());
	}
}

void Shader::Bind() const
{
	glUseProgram(m_id);
}

void Shader::UnBind() const
{
	glUseProgram(0);
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& mat) const
{
	int uniformId = glGetUniformLocation(m_id, name.c_str());

	if (uniformId != -1)
		glUniformMatrix4fv(uniformId, 1, false, &mat[0][0]);
	else
		LOG_WARNING("Uniform: \"%s\" doesn't exist", name.c_str());
}

void Shader::SetUniform1iv(const std::string& name, int count, const int* data) const
{
	int uniformId = glGetUniformLocation(m_id, name.c_str());

	if (uniformId != -1)
		glUniform1iv(uniformId, count, data);
	else
		LOG_WARNING("Uniform: \"%s\" doesn't exist", name.c_str());
}

void Shader::SetUniform1i(const std::string& name, int data) const
{
	int uniformId = glGetUniformLocation(m_id, name.c_str());

	if (uniformId != -1)
		glUniform1i(uniformId, data);
	else
		LOG_WARNING("Uniform: \"%s\" doesn't exist", name.c_str());
}

void Shader::SetUniform1f(const std::string& name, float data) const
{
	int uniformId = glGetUniformLocation(m_id, name.c_str());

	if (uniformId != -1)
		glUniform1f(uniformId, data);
	else
		LOG_WARNING("Uniform: \"%s\" doesn't exist", name.c_str());
}

void Shader::SetUniformVec2(const std::string& name, const glm::vec2& data) const
{
	int uniformId = glGetUniformLocation(m_id, name.c_str());

	if (uniformId != -1)
		glUniform2f(uniformId, data.x, data.y);
	else
		LOG_WARNING("Uniform: \"%s\" doesn't exist", name.c_str());
}

void Shader::SetUniformVec3(const std::string& name, const glm::vec3& data) const
{
	int uniformId = glGetUniformLocation(m_id, name.c_str());

	if (uniformId != -1)
		glUniform3f(uniformId, data.x, data.y, data.z);
	else
		LOG_WARNING("Uniform: \"%s\" doesn't exist", name.c_str());
}

void Shader::SetUniformVec4(const std::string& name, const glm::vec4& data) const
{
	int uniformId = glGetUniformLocation(m_id, name.c_str());

	if (uniformId != -1)
		glUniform4f(uniformId, data.x, data.y, data.z, data.w);
	else
		LOG_WARNING("Uniform: \"%s\" doesn't exist", name.c_str());
}

void Shader::SetUniformVec2v(const std::string& name, int count, const glm::vec2* data) const
{
	int uniformId = glGetUniformLocation(m_id, name.c_str());

	if (uniformId != -1)
		glUniform2fv(uniformId, count, (const float*)data);
	else
		LOG_WARNING("Uniform: \"%s\" doesn't exist", name.c_str());
}

void Shader::SetUniformVec3v(const std::string& name, int count, const glm::vec3* data) const
{
	int uniformId = glGetUniformLocation(m_id, name.c_str());

	if (uniformId != -1)
		glUniform3fv(uniformId, count, (const float*)data);
	else
		LOG_WARNING("Uniform: \"%s\" doesn't exist", name.c_str());
}
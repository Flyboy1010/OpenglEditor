#pragma once

#include <unordered_map>
#include <memory>
#include "Renderer/Shader.h"
#include "Renderer/Model.h"

class AssetsManager
{
public:
	static AssetsManager& Get();

	void LoadShader(const std::string& name, const std::string& filepath);
	void LoadModel(const std::string& name, const std::string& filepath);

	Shader* GetShader(const std::string& name);
	const Model* GetModel(const std::string& name);

private:
	AssetsManager() {}
	~AssetsManager() {}

private:
	std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaders;
	std::unordered_map<std::string, std::unique_ptr<Model>> m_models;
};
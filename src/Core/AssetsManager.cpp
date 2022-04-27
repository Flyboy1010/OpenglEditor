#include "Core/AssetsManager.h"

AssetsManager& AssetsManager::Get()
{
	static AssetsManager instance;
	return instance;
}

void AssetsManager::LoadShader(const std::string& name, const std::string& filepath)
{
	m_shaders[name] = std::make_unique<Shader>(filepath);
}

void AssetsManager::LoadModel(const std::string& name, const std::string& filepath)
{
	m_models[name] = std::make_unique<Model>(filepath);
}

Shader* AssetsManager::GetShader(const std::string& name)
{
	auto it = m_shaders.find(name);

	if (it != m_shaders.end())
		return it->second.get();

	return nullptr;
}

const Model* AssetsManager::GetModel(const std::string& name)
{
	auto it = m_models.find(name);

	if (it != m_models.end())
		return it->second.get();

	return nullptr;
}

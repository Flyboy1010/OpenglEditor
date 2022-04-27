#pragma once

#include "Scene.h"
#include <yaml-cpp/yaml.h>

class SceneSerializer
{
public:
	SceneSerializer(Scene* scene);
	~SceneSerializer();

	void Serialize(const std::string& filepath);

	void Deserialize(const std::string& filepath);

private:
	Scene* m_scene;
};
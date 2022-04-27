#pragma once

#include <string>
#include <glm/glm.hpp>
#include <vector>
#include "Core/Core.h"
#include "Core/EditorCamera.h"

struct Entity
{
	std::string name;
	glm::mat4 transform;
	const Model* model;
};

class Scene
{
public:
	Scene(const std::string& name);
	~Scene();

	const std::string& GetName() const { return m_name; }
	std::vector<Entity>& GetEntities() { return m_entities; }

	void AddEntity(const std::string& name, const Model* model);

	void Render(const EditorCamera& camera, Shader* shader);

private:
	std::string m_name;
	std::vector<Entity> m_entities;

	friend class SceneSerializer;
};
#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Core/Renderer/Model.h"
#include "Core/EditorCamera.h"

class Entity
{
public:
	Entity(const std::string& name, const Model* model);
	~Entity();

	const Model* GetModel() const { return m_model; }
	std::vector<Material>& GetMaterials() { return m_materials; }

	void Render(const EditorCamera& camera, Shader* shader);

private:
	std::string m_name;
	const Model* m_model;
	std::vector<Material> m_materials;
	glm::mat4 m_transform;
};


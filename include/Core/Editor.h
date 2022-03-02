#pragma once

#include "Core.h"
#include "Core/Model.h"
#include "Core/Camera.h"
#include "Core/EditorCamera.h"
#include <memory>
#include <vector>
#include <list>

struct Entity
{
	glm::mat4 transform;
	Model* model;
};

class Editor
{
public:
	Editor();
	~Editor();

	void Init();

	void Update(float delta);
	void FixedUpdate(float delta);

	void Render();
	void ImGuiRender();

private:
	void AddEntity(const std::string& modelPath);

private:
	std::list<std::unique_ptr<Model>> m_models;
	std::vector<Entity> m_entities;

	std::unique_ptr<Shader> m_modelShader;
	std::unique_ptr<Framebuffer> m_viewportFB;
	std::unique_ptr<Framebuffer> m_shadowMapFB;

	EditorCamera m_camera;
	
	glm::vec2 m_viewportWindowPosition;
	glm::vec2 m_viewportWindowSize;
	glm::vec2 m_viewportSize;

	unsigned int m_entitySelectedIndex;
	bool m_canSelectEntity;
	
	int m_command;
};
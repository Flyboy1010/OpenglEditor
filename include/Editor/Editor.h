#pragma once

#include "Core/Core.h"
#include <memory>
#include <vector>
#include "Core/EditorCamera.h"
#include "Scene/Scene.h"

class Editor
{
public:
	Editor();
	~Editor();

	void Init();

	void Update(float delta);

	void Render();
	void RenderImGui();

private:
	void AddEntity(const std::string& name, const std::string& modelPath);
	void SaveScene(const std::string& filepath);
	void LoadScene(const std::string& filepath);

private:
	std::vector<std::unique_ptr<Model>> m_models;
	std::unique_ptr<Shader> m_modelShader;
	
	std::unique_ptr<Framebuffer> m_viewportFB;
	glm::vec2 m_viewportWindowPosition;
	glm::vec2 m_viewportWindowSize;
	glm::vec2 m_viewportSize;

	EditorCamera m_camera;

	std::unique_ptr<Scene> m_currentScene;

	unsigned int m_entitySelectedIndex;
	bool m_canSelectEntity;
	
	int m_command;
};
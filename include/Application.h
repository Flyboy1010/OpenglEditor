#pragma once

#include "Core/Core.h"
#include "Editor/Editor.h"

struct ApplicationSettings
{
	int windowWidth;
	int windowHeight;
	bool fullscreen;
	bool vsync;

	ApplicationSettings();

	void SaveConfig(const std::string& filepath);
	void LoadConfig(const std::string& filepath);
};

class Application
{
public:
	Application();
	~Application();

	int Init(const ApplicationSettings& settings);
	void Run();
	void Stop();

private:
	void Update(float delta);
	void FixedUpdate(float delta);
	void Render();
	bool IsRunning();

private:
	ApplicationSettings m_settings;
	GLFWwindow* m_window;

	Editor m_editor;

	bool m_running;
};

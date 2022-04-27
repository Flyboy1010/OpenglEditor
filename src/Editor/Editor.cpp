#include "Editor/Editor.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>
#include <fstream>
#include "Scene/SceneSerializer.h"

Editor::Editor()
{
	m_modelShader = nullptr;
	m_viewportFB = nullptr;
	m_entitySelectedIndex = -1;
	m_command = ImGuizmo::OPERATION::TRANSLATE;
	m_canSelectEntity = true;
	m_currentScene = nullptr;
}

Editor::~Editor()
{

}

void Editor::Init()
{
	// load model shader

	m_modelShader = std::make_unique<Shader>("Assets/Shaders/PBR.glsl");

	// create viewport framebuffer

	FramebufferSpecification viewportFbDeff;
	viewportFbDeff.width = 1280;
	viewportFbDeff.height = 720;
	viewportFbDeff.fbAttachments = { FramebufferAttachmentFormat::RGBA8, FramebufferAttachmentFormat::RED, FramebufferAttachmentFormat::DEPTH };

	m_viewportFB = std::make_unique<Framebuffer>(viewportFbDeff);

	// create scene

	m_currentScene = std::make_unique<Scene>("New Scene");
	
	// add entities

	AddEntity("stormtrooper", "Assets/Models/Stormtrooper/scene.gltf");
	AddEntity("tiger1", "Assets/Models/tiger1/scene.gltf");
	AddEntity("sponza", "Assets/Models/sponza/scene.gltf");
	AddEntity("mask", "Assets/Models/mask/scene.gltf");
	AddEntity("helmet", "Assets/Models/corinthian_helmet/scene.gltf");
}

void Editor::AddEntity(const std::string& name, const std::string& modelPath)
{
	// check if the model is already loaded

	for (auto& model : m_models)
	{
		if (model->GetPath() == modelPath)
		{
			m_currentScene->AddEntity(name, model.get());
			return;
		}
	}

	// if not then load it

	Model* model = new Model(modelPath);
	m_models.push_back(std::unique_ptr<Model>(model));
	m_currentScene->AddEntity(name, model);
}

void Editor::SaveScene(const std::string& filepath)
{
	SceneSerializer out(m_currentScene.get());

	out.Serialize(filepath);
}

void Editor::LoadScene(const std::string& filepath)
{

}

void Editor::Update(float delta)
{
	m_camera.Update(delta);

	if (Input::KeyPressed(GLFW_KEY_Q))
		m_command = ImGuizmo::OPERATION::TRANSLATE;
	else if (Input::KeyPressed(GLFW_KEY_W))
		m_command = ImGuizmo::OPERATION::SCALE;
	else if (Input::KeyPressed(GLFW_KEY_E))
		m_command = ImGuizmo::OPERATION::ROTATE;
}

void Editor::Render()
{
	PROFILE(__FUNCTION__);

	// bind the viewport framebuffer

	m_viewportFB->Bind();

	// set viewport and clear

	glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// clear the "id buffer" (color attachment 1) to -1 (this is where the entities id are stored)

	m_viewportFB->ClearColorAttachment(1, -1);

	// render all entities and set id as uniform to the shader

	m_currentScene->Render(m_camera, m_modelShader.get());

	// mouse entity selection

	if (m_canSelectEntity && !m_camera.IsUsing())
	{
		if (Input::MouseButtonJustPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			float xMouse, yMouse;
			Input::GetMousePos(&xMouse, &yMouse);

			xMouse -= m_viewportWindowPosition.x;
			yMouse -= m_viewportWindowPosition.y;

			if (xMouse >= 0 && xMouse < m_viewportWindowSize.x && yMouse >= m_viewportWindowSize.y - m_viewportSize.y && yMouse < m_viewportWindowSize.y)
			{
				int id;
				m_viewportFB->ReadPixels(1, (int)xMouse, m_viewportWindowSize.y - (int)yMouse - 1, 1, 1, &id);
				m_entitySelectedIndex = id;
			}
		}
	}

	// unbind the framebuffer

	m_viewportFB->UnBind();
}

void Editor::RenderImGui()
{
	// set up dockspace

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	ImGui::ShowDemoWindow();

	/* MENU BAR */

	{
		ImGui::BeginMainMenuBar();

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene", "(CTRL+S)"))
			{
				m_currentScene = std::make_unique<Scene>("New Scene");
				m_entitySelectedIndex = -1;
			}

			if (ImGui::MenuItem("Save Scene", "(CTRL+S)"))
				SaveScene("scene1.yaml");

			if (ImGui::MenuItem("Load Scene", "(CTRL+L)")) {}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	/* SCENE WINDOW */

	{
		ImGui::Begin("Scene hierarchy");

		if (ImGui::TreeNodeEx(m_currentScene->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			auto& entities = m_currentScene->GetEntities();

			for (int i = 0; i < entities.size(); i++)
			{
				if (ImGui::Selectable(entities[i].name.c_str(), m_entitySelectedIndex == i))
					m_entitySelectedIndex = i;
			}

			ImGui::TreePop();
		}

		ImGui::End();
	}

	/* PROPERTIES WINDOW */

	{
		ImGui::Begin("Properties");

		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("\tTranslation");
			ImGui::Text("\tRotation");
			ImGui::Text("\tScale");
		}

		ImGui::End();
	}

	/* STATS */

	{
		ImGui::Begin("Stats");

		const RendererStats& stats = Renderer::GetStats();

		ImGui::Text("Frame Time: %.3fms, FPS: %.1f", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Draw calls: %d", stats.drawCallsCount);
		ImGui::Text("Quads rendered: %d", stats.quadsCount);
		ImGui::Text("Lines rendered: %d", stats.linesCount);

		Renderer::ResetStats();

		ImGui::End();
	}

	/* PROFILER */

	{
		ImGui::Begin("Profiler");

		auto& profiles = Profiler::Get().GetProfiles();

		for (auto& profile : profiles)
		{
			ImGui::Text("%s: %.3fms", profile.name.c_str(), profile.duration);
		}

		Profiler::Get().ClearProfiles();

		ImGui::End();
	}

	/* LOGS */

	{
		ImGui::Begin("Logs");

		if (ImGui::Button("Clear logs"))
			Logger::Get().ClearLogs();

		ImGui::SameLine();

		if (ImGui::Button("Save logs"))
		{
			const std::vector<Log>& logs = Logger::Get().GetLogs();

			std::ofstream file("logs.txt");

			if (file.is_open())
			{
				for (auto& log : logs)
				{
					switch (log.type)
					{
					case LogType::INFO:
						file << "[INFO] " << log.msg << std::endl;
						break;
					case LogType::WARNING:
						file << "[WARNING] " << log.msg << std::endl;
						break;
					case LogType::ERROR:
						file << "[ERROR] " << log.msg << std::endl;
						break;
					case LogType::ASSET:
						file << "[ASSET] " << log.msg << std::endl;
						break;
					}
				}

				file.close();
			}
		}

		ImGui::Separator();

		ImGui::BeginChild("logs", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		const std::vector<Log>& logs = Logger::Get().GetLogs();

		ImGuiListClipper clipper;
		clipper.Begin(logs.size());
		while (clipper.Step())
		{
			for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
			{
				const Log& log = logs[i];

				switch (log.type)
				{
				case LogType::INFO:
					ImGui::TextColored(ImVec4(0.33f, 0.67f, 0.33f, 1), ("[INFO] " + log.msg).c_str());
					break;
				case LogType::WARNING:
					ImGui::TextColored(ImVec4(0.92f, 0.76f, 0, 1), ("[WARNING] " + log.msg).c_str());
					break;
				case LogType::ERROR:
					ImGui::TextColored(ImVec4(0.85f, 0.4f, 0.26f, 1), ("[ERROR] " + log.msg).c_str());
					break;
				case LogType::ASSET:
					ImGui::TextColored(ImVec4(0.48f, 0.41f, 0.85f, 1), ("[ASSET] " + log.msg).c_str());
					break;
				}
			}
		}

		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();

		ImGui::End();
	}

	/* VIEWPORT */

	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGui::Begin("Viewport");

		ImVec2 windowSize = ImGui::GetContentRegionAvail();

		// check if the viewport size has changed

		if (m_viewportSize != *(glm::vec2*)&windowSize)
		{
			// resize the viewport framebuffer

			m_viewportSize = *(glm::vec2*)&windowSize;
			if (m_viewportSize.x > 0 && m_viewportSize.y > 0)
				m_viewportFB->Resize(m_viewportSize.x, m_viewportSize.y);

			// change camera aspect ratio

			m_camera.SetViewport(m_viewportSize.x, m_viewportSize.y);
		}

		m_viewportWindowPosition = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
		m_viewportWindowSize = { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() };

		// draw the color buffer 0 from the viewport framebuffer

		ImGui::Image((void*)m_viewportFB->GetColorAttachment(0), windowSize, ImVec2(0, 1), ImVec2(1, 0));

		// guizmos (if there is a selected entity) and the camera is not being used

		if (m_entitySelectedIndex != -1 && !m_camera.IsUsing())
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

			Entity& selectedEntity = m_currentScene->GetEntities()[m_entitySelectedIndex];
			ImGuizmo::Manipulate(glm::value_ptr(m_camera.GetTransform()), glm::value_ptr(m_camera.GetProjection()), (ImGuizmo::OPERATION)m_command, ImGuizmo::LOCAL, glm::value_ptr(selectedEntity.transform));
		
			// while the mouse is over the gizmo or the gizmo is moving then you cant select an entity

			if (ImGuizmo::IsUsing() || ImGuizmo::IsOver((ImGuizmo::OPERATION)m_command))
				m_canSelectEntity = false;
			else
				m_canSelectEntity = true;
		}

		ImGui::End();

		ImGui::PopStyleVar();
	}
}

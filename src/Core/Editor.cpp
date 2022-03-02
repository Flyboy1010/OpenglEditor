#include "Core/Editor.h"
#include "Core/Core.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

Editor::Editor()
{
	m_modelShader = nullptr;
	m_viewportFB = nullptr;
	m_shadowMapFB = nullptr;
	m_entitySelectedIndex = -1;
	m_command = ImGuizmo::OPERATION::TRANSLATE;
	m_canSelectEntity = true;
}

Editor::~Editor()
{

}

void Editor::Init()
{
	// load model shader

	m_modelShader = std::make_unique<Shader>("Shaders/modelPBR.vert", "Shaders/modelPBR.frag");

	// create viewport framebuffer

	FramebufferDeff viewportFbDeff;
	viewportFbDeff.width = 1280;
	viewportFbDeff.height = 720;
	viewportFbDeff.fbAttachments = { FramebufferAttachment::RGBA8, FramebufferAttachment::RED, FramebufferAttachment::DEPTH };

	m_viewportFB = std::make_unique<Framebuffer>(viewportFbDeff);

	// create shadowMap framebuffer

	FramebufferDeff shadowMapFbDeff;
	shadowMapFbDeff.width = 1280;
	shadowMapFbDeff.height = 720;
	shadowMapFbDeff.fbAttachments = { FramebufferAttachment::DEPTH };

	m_shadowMapFB = std::make_unique<Framebuffer>(shadowMapFbDeff);
	
	// add entities

	/*std::string path = "Models/chess/obj";
	for (const auto& entry : std::filesystem::directory_iterator(path))
		AddEntity("Models/chess/obj/bishop_01.obj");*/

	AddEntity("Models/sponza/scene.gltf");
	AddEntity("Models/tiger1/scene.gltf");
	AddEntity("Models/Stormtrooper/scene.gltf");
	AddEntity("Models/mask/scene.gltf");
	AddEntity("Models/corinthian_helmet/scene.gltf");
	AddEntity("Models/pilot_avatar/scene.gltf");
	AddEntity("Models/barrels/scene.gltf");
	//AddEntity("Models/roman_helmet/scene.gltf");
}

void Editor::AddEntity(const std::string& modelPath)
{
	// check if the model is already loaded

	for (auto& model : m_models)
	{
		if (model->GetPath() == modelPath)
		{
			m_entities.push_back({ glm::mat4(1.0f), model.get() });
			return;
		}
	}

	// if not then load it

	Model* model = new Model(modelPath);
	m_models.push_back(std::unique_ptr<Model>(model));
	m_entities.push_back({ glm::mat4(1.0f), model });
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

	if (Input::KeyJustPressed(GLFW_KEY_SPACE))
		LOG_WARNING("Test");
}

void Editor::FixedUpdate(float delta)
{

}

void Editor::Render()
{
	// bind the viewport framebuffer

	m_viewportFB->Bind();

	// set viewport and clear

	glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// clear the "id buffer" (color attachment 1) to -1 (this is where the entities id are stored)

	m_viewportFB->ClearColorAttachment(1, -1);

	// render all entities and set id as uniform to the shader

	for (int i = 0; i < m_entities.size(); i++)
	{
		Entity& e = m_entities[i];
		m_modelShader->Bind();
		m_modelShader->SetUniform1i("u_id", i);
		Renderer::RenderModel(e.model, e.transform, m_camera.GetProjection(), m_camera.GetTransform(), m_camera.GetPosition(), m_modelShader.get());
	}

	// mouse entity selection

	if (m_canSelectEntity)
	{
		if (Input::MouseButtonJustPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			float xMouse, yMouse;
			Input::GetMousePos(&xMouse, &yMouse);

			xMouse -= m_viewportWindowPosition.x;
			yMouse -= m_viewportWindowPosition.y;

			if (xMouse >= 0 && xMouse < m_viewportWindowSize.x && yMouse >= m_viewportWindowSize.y - m_viewportSize.y && yMouse < m_viewportWindowSize.y)
			{
				int id = m_viewportFB->ReadPixel(1, (int)xMouse, m_viewportWindowSize.y - (int)yMouse - 1);
				m_entitySelectedIndex = id;
			}
		}
	}

	// unbind the framebuffer

	m_viewportFB->UnBind();
}

void Editor::ImGuiRender()
{
	// set up dockspace ?

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	// ImGui::ShowDemoWindow();

	/* SCENE WINDOW */

	{
		ImGui::Begin("Scene");

		for (int i = 0; i < m_entities.size(); i++)
		{
			const std::string& entityModelPath = m_entities[i].model->GetPath();
			int offset0 = entityModelPath.find_last_of('/');
			int offset1 = entityModelPath.find_last_of('/', offset0 - 1);
			std::string entityName = entityModelPath.substr(offset1 + 1, offset0 - offset1 - 1);

			if (ImGui::Selectable(entityName.c_str(), m_entitySelectedIndex == i))
				m_entitySelectedIndex = i;
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

		ImGui::Text("Draw calls: %d", stats.drawCalls);
		ImGui::Text("Quads rendered: %d", stats.quadsCount);
		ImGui::Text("Lines rendered: %d", stats.linesCount);

		Renderer::ResetStats();

		ImGui::End();
	}

	/* Log */

	{
		ImGui::Begin("Log");

		if (ImGui::Button("Clear logs"))
			Logger::Get().ClearLogs();

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

		// guizmos (if there is a selected entity)

		if (m_entitySelectedIndex != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

			ImGuizmo::Manipulate(glm::value_ptr(m_camera.GetTransform()), glm::value_ptr(m_camera.GetProjection()), (ImGuizmo::OPERATION)m_command, ImGuizmo::LOCAL, glm::value_ptr(m_entities[m_entitySelectedIndex].transform));
		
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

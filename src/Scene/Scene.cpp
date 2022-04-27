#include "Scene/Scene.h"

Scene::Scene(const std::string& name)
{
	m_name = name;
}

Scene::~Scene()
{

}

void Scene::AddEntity(const std::string& name, const Model* model)
{
	m_entities.push_back({ name, glm::mat4(1.0f), model });
}

void Scene::Render(const EditorCamera& camera, Shader* shader)
{
	for (int i = 0; i < m_entities.size(); i++)
	{
		const Entity& e = m_entities[i];
		shader->Bind();
		shader->SetUniform1i("u_id", i);
		Renderer::RenderModel(e.model, e.transform, camera.GetProjection(), camera.GetTransform(), camera.GetPosition(), shader);
	}
}

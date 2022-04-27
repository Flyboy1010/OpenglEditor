#include "Scene/Entity.h"
#include "Core/Renderer/Renderer.h"

Entity::Entity(const std::string& name, const Model* model)
{
	m_name = name;
	m_model = model;
	m_transform = glm::mat4(1.0f);
	m_materials = model->GetMaterials();
}

Entity::~Entity()
{
}

void Entity::Render(const EditorCamera& camera, Shader* shader)
{
	auto& meshes = m_model->GetMeshes();

	for (int i = 0; i < meshes.size(); i++)
	{
		auto& mesh = meshes[i];
		const Material& material = m_materials[i];

		Renderer::RenderMesh(mesh.get(), m_transform, camera.GetProjection(), camera.GetTransform(), camera.GetPosition(), shader, material);
	}
}

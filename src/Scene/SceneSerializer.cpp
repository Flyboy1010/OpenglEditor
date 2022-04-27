#include "Scene/SceneSerializer.h"
#include <yaml-cpp/yaml.h>
#include <fstream>

/* Serialize and deserialize entity */

static void SerializeEntity(YAML::Emitter& out, const Entity& entity)
{
	out << YAML::BeginMap; // entity map
	out << YAML::Key << "name" << YAML::Value << entity.name; // entity name
	out << YAML::Key << "model" << YAML::Value << entity.model->GetPath(); // entity model path
	out << YAML::EndMap; // end entity map
}

static void DeserializeEntity(YAML::Node& node, Entity& entity)
{
	entity.name = node["name"].as<std::string>();
}

/* SCENE SERIALIZER CLASS */

SceneSerializer::SceneSerializer(Scene* scene)
{
	m_scene = scene;
}

SceneSerializer::~SceneSerializer()
{

}

void SceneSerializer::Serialize(const std::string& filepath)
{
	YAML::Emitter out;

	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << m_scene->m_name; // scene
	out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq; // entities array
	
	for (auto& entity : m_scene->m_entities)
		SerializeEntity(out, entity);

	out << YAML::EndSeq; // end entities array
	out << YAML::EndMap;

	// save to file

	std::ofstream file(filepath);
	file << out.c_str();
}

void SceneSerializer::Deserialize(const std::string& filepath)
{
	std::ifstream file(filepath);
	std::stringstream ss;
	ss << file.rdbuf();
	file.close();

	// root node

	YAML::Node root = YAML::Load(ss.str());


}

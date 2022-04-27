#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <assimp/scene.h>
#include "Shader.h"
#include "Texture.h"
#include "Buffer.h"
#include "VertexArray.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureUV;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

struct Material
{
	Material();

	const Texture* albedoMap;
	const Texture* normalMap;
	const Texture* metallicRoughnessMap;
	bool usingAlbedoMap;
	bool usingNormalMap;
	bool usingmetallicRoughnessMap;
	glm::vec3 albedoColor;
	float metallic;
	float roughness;
};

struct Mesh
{
	Mesh(const std::vector<Vertex>& vertexData, const std::vector<unsigned int>& indexData);
	~Mesh();

	VertexBuffer vb;
	IndexBuffer ib;
	VertexArray va;
};

class Model
{
public:
	Model(const std::string& path);
	~Model();

	const std::string& GetPath() const { return m_path; }
	const std::vector<std::unique_ptr<Mesh>>& GetMeshes() const { return m_meshes; }
	const std::vector<Material>& GetMaterials() const { return m_materials; }

private:
	Material LoadMaterial(const aiMaterial* assimpMaterial);
	Texture* LoadTexture(const aiMaterial* assimpMaterial, aiTextureType textureType);

private:
	std::string m_path;
	std::vector<std::unique_ptr<Mesh>> m_meshes;
	std::vector<Material> m_materials;
	std::vector<std::unique_ptr<Texture>> m_textures;
};
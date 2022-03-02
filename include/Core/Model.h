#pragma once

#include <glm/glm.hpp>
#include <string>
#include "Gfx/Shader.h"
#include "Gfx/Texture.h"
#include <list>
#include <vector>
#include <assimp/scene.h>

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
	const Texture* albedoMap;
	const Texture* normalMap;
	const Texture* metallicRoughnessMap;
	const Texture* aoMap;
};

struct Mesh
{
	Mesh(const std::vector<Vertex>& vertexData, const std::vector<unsigned int>& indexData, const Material& meshMaterial);
	~Mesh();

	unsigned int va, vb, ib;
	unsigned int indicesCount;
	Material material;
};

class Model
{
public:
	Model(const std::string& path);
	~Model();

	const std::string& GetPath() const { return m_path; }
	const std::vector<std::unique_ptr<Mesh>>& GetMeshes() const { return m_meshes; }

private:
	Texture* LoadTexture(const aiMaterial* assimpMaterial, aiTextureType textureType);

private:
	std::string m_path;
	std::vector<std::unique_ptr<Mesh>> m_meshes;
	std::list<std::unique_ptr<Texture>> m_textures;
};
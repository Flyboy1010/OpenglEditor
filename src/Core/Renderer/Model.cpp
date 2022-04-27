#include "Core/Renderer/Model.h"
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Core/Debug/Logger.h"

/* MATERIAL */

Material::Material()
{
	albedoMap = nullptr;
	normalMap = nullptr;
	metallicRoughnessMap = nullptr;
	usingAlbedoMap = false;
	usingNormalMap = false;
	usingmetallicRoughnessMap = false;
	albedoColor = { 0.0f, 0.0f, 0.0f };
	metallic = 0.0f;
	roughness = 0.0f;
}

/* MESH */

Mesh::Mesh(const std::vector<Vertex>& vertexData, const std::vector<unsigned int>& indexData)
{
	// vertex buffer

	vb.Create(vertexData.size() * sizeof(Vertex), vertexData.data());

	// vertex buffer layout

	VertexBufferLayout layout;
	layout.AddElement<float>(3); // position
	layout.AddElement<float>(3); // normal
	layout.AddElement<float>(2); // textureUV
	layout.AddElement<float>(3); // tangent
	layout.AddElement<float>(3); // bitangent

	// vertex array

	va.Create(vb, layout);

	// index buffer

	ib.Create(indexData.size(), indexData.data());
}

Mesh::~Mesh()
{

}

/* MODEL */

Model::Model(const std::string& path)
{
	Assimp::Importer assimpImporter;

	const aiScene* assimpScene = assimpImporter.ReadFile(path, aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);

	if (assimpScene == nullptr)
	{
		LOG_ERROR("Couldn't load %s", path.c_str());
		return;
	}

	m_path = path;

	// for each mesh

	for (int i = 0; i < assimpScene->mNumMeshes; i++)
	{
		const aiMesh* assimpMesh = assimpScene->mMeshes[i];

		// vertex data

		std::vector<Vertex> vertexData;
		vertexData.resize(assimpMesh->mNumVertices);

		for (unsigned int j = 0; j < assimpMesh->mNumVertices; j++)
		{
			// vertex position

			vertexData[j].position.x = assimpMesh->mVertices[j].x;
			vertexData[j].position.y = assimpMesh->mVertices[j].y;
			vertexData[j].position.z = assimpMesh->mVertices[j].z;

			// vertex normal

			vertexData[j].normal.x = assimpMesh->mNormals[j].x;
			vertexData[j].normal.y = assimpMesh->mNormals[j].y;
			vertexData[j].normal.z = assimpMesh->mNormals[j].z;

			// vertex textureUV

			if (assimpMesh->mTextureCoords[0] != nullptr)
			{
				vertexData[j].textureUV.x = assimpMesh->mTextureCoords[0][j].x;
				vertexData[j].textureUV.y = assimpMesh->mTextureCoords[0][j].y;
			}

			// tangents

			vertexData[j].tangent.x = assimpMesh->mTangents[j].x;
			vertexData[j].tangent.y = assimpMesh->mTangents[j].y;
			vertexData[j].tangent.z = assimpMesh->mTangents[j].z;

			// bitangents

			vertexData[j].bitangent.x = assimpMesh->mBitangents[j].x;
			vertexData[j].bitangent.y = assimpMesh->mBitangents[j].y;
			vertexData[j].bitangent.z = assimpMesh->mBitangents[j].z;
		}

		// index data

		std::vector<unsigned int> indexData;
		indexData.resize(3 * assimpMesh->mNumFaces);

		for (unsigned int j = 0; j < assimpMesh->mNumFaces; j++)
		{
			const aiFace* assimpFace = &assimpMesh->mFaces[j];

			indexData[j * 3 + 0] = assimpFace->mIndices[0];
			indexData[j * 3 + 1] = assimpFace->mIndices[1];
			indexData[j * 3 + 2] = assimpFace->mIndices[2];
		}

		// load material

		const aiMaterial* assimpMaterial = assimpScene->mMaterials[assimpMesh->mMaterialIndex];
		m_materials.push_back(LoadMaterial(assimpMaterial));

		// construct the mesh

		m_meshes.push_back(std::make_unique<Mesh>(vertexData, indexData));
	}
}

Model::~Model()
{

}

Material Model::LoadMaterial(const aiMaterial* assimpMaterial)
{
	Material m;

	m.albedoMap = LoadTexture(assimpMaterial, aiTextureType_DIFFUSE);
	m.normalMap = LoadTexture(assimpMaterial, aiTextureType_NORMALS);
	m.metallicRoughnessMap = LoadTexture(assimpMaterial, aiTextureType_UNKNOWN);

	// if not has an albedo map

	if (m.albedoMap != nullptr)
		m.usingAlbedoMap = true;

	// if not has an normal map

	if (m.normalMap != nullptr)
		m.usingNormalMap = true;

	// if not has an metallic map

	if (m.metallicRoughnessMap != nullptr)
		m.usingmetallicRoughnessMap = true;

	return m;
}

Texture* Model::LoadTexture(const aiMaterial* assimpMaterial, aiTextureType textureType)
{
	if (assimpMaterial->GetTextureCount(textureType) <= 0)
		return nullptr;

	// get the relative path(from the model file) to the texture

	aiString relativePath;
	assimpMaterial->GetTexture(textureType, 0, &relativePath);

	// construct the "absolute" path(from the exe itself) to the texture

	std::string path = m_path.substr(0, m_path.find_last_of('/')) + '/' + relativePath.C_Str();

	// check if the texture is already loaded

	for (auto& texture : m_textures)
	{
		if (texture->GetPath() == path)
			return texture.get();
	}

	// if not loaded then load it and add to the texture list

	Texture* texture = new Texture(path);
	m_textures.push_back(std::unique_ptr<Texture>(texture));

	// return the texture

	return texture;
}

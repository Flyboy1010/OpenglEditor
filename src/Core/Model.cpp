#include "Core/Model.h"
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/* MESH */

Mesh::Mesh(const std::vector<Vertex>& vertexData, const std::vector<unsigned int>& indexData, const Material& meshMaterial)
{
	indicesCount = indexData.size();
	material = meshMaterial;

	// vertex array

	glGenVertexArrays(1, &va);
	glBindVertexArray(va);

	// vertex buffer

	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex), vertexData.data(), GL_STATIC_DRAW);

	// vertex buffer layout

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, textureUV));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, bitangent));

	// index buffer

	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(unsigned int), indexData.data(), GL_STATIC_DRAW);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &va);
	glDeleteBuffers(1, &vb);
	glDeleteBuffers(1, &ib);
}

/* MODEL */

Model::Model(const std::string& path)
{
	Assimp::Importer assimpImporter;

	const aiScene* assimpScene = assimpImporter.ReadFile(path, aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);

	if (assimpScene == nullptr)
	{
		std::cout << "Couldn't load " << path << std::endl;
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

			vertexData[j].textureUV.x = assimpMesh->mTextureCoords[0][j].x;
			vertexData[j].textureUV.y = assimpMesh->mTextureCoords[0][j].y;

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

		// load textures

		const aiMaterial* assimpMaterial = assimpScene->mMaterials[assimpMesh->mMaterialIndex];

		Material material;

		/*material.textureDiffuse = LoadTexture(assimpMaterial, aiTextureType_DIFFUSE);
		material.textureNormal = LoadTexture(assimpMaterial, aiTextureType_NORMALS);*/

		material.albedoMap = LoadTexture(assimpMaterial, aiTextureType_DIFFUSE);
		material.normalMap = LoadTexture(assimpMaterial, aiTextureType_NORMALS);
		material.metallicRoughnessMap = LoadTexture(assimpMaterial, aiTextureType_UNKNOWN);
		material.aoMap = LoadTexture(assimpMaterial, aiTextureType_AMBIENT);

		// construct the mesh

		m_meshes.push_back(std::make_unique<Mesh>(vertexData, indexData, material));
	}
}

Model::~Model()
{

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

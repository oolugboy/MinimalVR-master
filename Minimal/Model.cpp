#include "Model.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
#include <Windows.h>

Model::Model(GLchar* path)
{
	this->toWorld = glm::mat4(1.0f);
	this->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -35.0f)) * toWorld;
	this->loadModel(path);
}
void Model::draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView)
{
	for (GLuint i = 0; i < this->meshes.size(); i++)
		this->meshes[i].draw(shaderProgram, projection, modelView, toWorld);
}
void Model::loadModel(string path)
{	
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		char buff[100];
		sprintf_s(buff, "ERROR::ASSIMP:: %s \n", import.GetErrorString());
		OutputDebugStringA(buff);
		return;
	}
	this->directory = path.substr(0, path.find_last_of('/'));

	this->processNode(scene->mRootNode, scene);
}
void Model::processNode(aiNode* node, const aiScene* scene)
{
	// Process all the node's meshes (if any)
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processMesh(mesh, scene));
	}
	// Then do the same for each of its children
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// Process vertex positions 
		glm::vec3 inPos;
		inPos.x = mesh->mVertices[i].x;
		inPos.y = mesh->mVertices[i].y;
		inPos.z = mesh->mVertices[i].z;
		vertex.position = inPos;

		//Now for the vertex normals 
		glm::vec3 inNorm;
		inNorm.x = mesh->mNormals[i].x;
		inNorm.y = mesh->mNormals[i].y;
		inNorm.z = mesh->mNormals[i].z;
		vertices.push_back(vertex);
	}
	// Process indices
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	glm::vec3 color;
	// Process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];
		aiColor3D diffuseColor(0.0f, 0.0f, 0.0f);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
		color.x = diffuseColor.r;
		color.y = diffuseColor.g;
		color.z = diffuseColor.b;		
	}
	return Mesh(vertices, indices, textures, color);
}
Model::~Model()
{
}

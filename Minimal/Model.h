#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	/*  Functions   */
	Model(GLchar* path);
	void draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView);
	void setToWorld(glm::mat4 toWorld);
private:
	/*  Model Data  */
	glm::mat4 toWorld; // Global transformation matrix
	vector<Mesh> meshes;
	string directory;
	/*  Functions   */
	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	~Model();
};


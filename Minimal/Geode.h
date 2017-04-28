#pragma once
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Geode
{
public:
	virtual void setToWorld(glm::mat4 toWorld) = 0;
	GLuint VBO, VAO, EBO, NBO, TBO;
	GLuint textureID;
	GLuint uColor;
	GLuint uProjection, uModelView;
	void printVector(glm::vec3 val);
	unsigned char * loadPPM(const char * fileName, int & width, int & height);
	glm::vec3 scaleVal;
	glm::mat4 toWorld;
	bool wired;	
	bool isCube;
	Geode();
	~Geode();
};


#pragma once
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Geode
{
public:
	virtual void setToWorld(glm::mat4 toWorld) = 0;
	GLuint VBO, VAO, EBO, NBO;
	GLuint uColor;
	GLuint uProjection, uModelView;
	void printVector(glm::vec3 val);
	glm::vec3 scaleVal;
	glm::mat4 toWorld;
	bool wired;	
	bool isCube;
	Geode();
	~Geode();
};


#pragma once
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Geode
{
public:
	void setToWorld(glm::mat4 toWorld);
	GLuint VBO, VAO, EBO, NBO;
	GLuint uColor;
	GLuint uProjection, uModelView;
	glm::vec3 scaleVal;
	glm::mat4 toWorld;
	bool wired;	
	Geode();
	~Geode();
};


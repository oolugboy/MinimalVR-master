#pragma once
#include "Geode.h"
#include <vector>

using namespace std;

class Line :public Geode
{
public:	
	glm::vec3 start;
	glm::vec3 end;
	vector< glm::vec3 > vertices;
	glm::vec3 dir;
	glm::vec3 color;
	void draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView);
	void loadLineVertices();
	const int vertNum = 2;
	void setToWorld(glm::mat4 toWorld);
	Line(glm::vec3 start);
	~Line();
};


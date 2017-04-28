#include "Cube.h"
#include <iostream>

using namespace std;

Cube::Cube(bool wired)
{
	this->wired = wired;
	toWorld = glm::mat4(1.0f);
	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);	
	glGenBuffers(1, &TBO);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
		
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	
	// We've sent the vertex data over to OpenGL, but there's still something missing.
	// In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);	

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);

	xMax = yMax = zMax = 2.0f; 
	xMin = yMin = zMin = -2.0f;
}

Cube::~Cube()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
void Cube::loadTextures(const char * fileName)
{	
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char * image;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	image = loadPPM(fileName, width, height);
	if(image == NULL)
		cout << " Failed to load the image " << endl;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Cube::setToWorld(glm::mat4 toWorld)
{
	this->toWorld = toWorld;
	this->toWorld = this->toWorld * glm::scale(glm::mat4(1.0f), scaleVal);
	cout << " The cube scale is " << toWorld[0][0] << endl;
	scaleBoundaries();
}

void Cube::scaleBoundaries()
{
	xMax = (xMax * toWorld[0][0]) + toWorld[3][0]; 
	xMin = (xMin * toWorld[0][0]) + toWorld[3][0];	
	yMax = (yMax * toWorld[0][0]) + toWorld[3][1]; 
	yMin = (yMin * toWorld[0][0]) + toWorld[3][1];
	zMax = (zMax * toWorld[0][0]) + toWorld[3][2]; 
	zMin = (zMin * toWorld[0][0]) + toWorld[3][2];
}
void Cube::draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 modelView)
{
	modelView = modelView * toWorld;
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelView = glGetUniformLocation(shaderProgram, "modelview");
	uColor = glGetUniformLocation(shaderProgram, "color");

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &(projection[0][0]));
	glUniformMatrix4fv(uModelView, 1, GL_FALSE, &(modelView[0][0]));
	glUniform3f(uColor, 1.0f, 0.0f, 0.0f);

	glBindTexture(GL_TEXTURE_2D, textureID);

	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);

	/* figure out the width of the lines*/
	if (wired)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}
void Cube::factorSphereCollision(glm::vec3 pos, float radius, glm::vec3 & velocity)
{
	if (pos.x + radius >= xMax || pos.x - radius <= xMin)
	{
		cout << " collided x " << endl;
		velocity.x *= -1.0f;
	}
	if (pos.y + radius >= yMax || pos.y - radius <= yMin)
	{
		cout << " collided y " << endl;
		velocity.y *= -1.0f;	
	}
	if (pos.z + radius >= zMax || pos.z - radius <= zMin)
	{
		cout << " collided z " << endl;
		velocity.z *= -1.0f;
	}
}
void Cube::update()
{
	spin(1.0f);
}

void Cube::spin(float deg)
{
	// If you haven't figured it out from the last project, this is how you fix spin's behavior
	toWorld = toWorld * glm::rotate(glm::mat4(1.0f), 1.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}
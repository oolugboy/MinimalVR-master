#include "Sphere.h"
#include <cmath>

using namespace std;

Sphere::Sphere(float radius, bool wired)
{

	toWorld = glm::mat4(1.0f); // Scale the toWorld accordingly 
	/* Set the number of longitudes and latitudes */
	latNum = 5;
	longNum = 5;

	m_radius = radius;
	this->wired = wired;
	currPos = glm::vec3(0, 0, 0);

	/* generate the sphere data */
	genVertices();
	genIndices();

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &NBO);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &(vertices[0]), GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.


	// We've sent the vertex and normal data over to OpenGL, but there's still something missing.
	// In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &(indices[0]), GL_STATIC_DRAW);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}
void Sphere::genVertices()
{
	for (int i = 0; i <= latNum; i++)
	{
		float theta = (i * PI) / latNum;

		for (int j = 0; j <= longNum; j++)
		{
			float phi = (j * 2 * PI) / longNum;
			float x = m_radius * sin(theta) * cos(phi);
			float y = m_radius * cos(theta);
			float z = m_radius * sin(theta) * sin(phi);

			glm::vec3 newPos = glm::vec3(x, y, z);
			vertices.push_back(newPos);
			normals.push_back(glm::normalize(newPos));
		}
	}
}

void Sphere::genIndices()
{
	for (int i = 0; i < latNum; i++)
	{
		for (int j = 0; j < longNum; j++)
		{
			int first = (i  * (longNum + 1)) + j;
			int second = first + (longNum + 1);
			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);

			indices.push_back(second);
			indices.push_back(second + 1);
			indices.push_back(first + 1);
		}
	}
}

/* Draw the sphere mainly for debugging purposes */
void Sphere::draw(GLint shaderProgram, glm::mat4 projection, glm::mat4 modelView)
{
	modelView = modelView * toWorld;
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelView = glGetUniformLocation(shaderProgram, "modelview");
	uColor = glGetUniformLocation(shaderProgram, "color");

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(uModelView, 1, GL_FALSE, &modelView[0][0]);
	glUniform3f(uColor, 1.0f, 1.0f, 1.0f);

	// Now draw the sphere. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);

	glEnable(GL_DEPTH_TEST);

	/* figure out the width of the lines*/
	if (wired)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#define FAIL(X) throw std::runtime_error(X)

using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

// Import the most commonly used types into the default namespace
using glm::ivec3;
using glm::ivec2;
using glm::uvec2;
using glm::mat3;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::quat;

namespace glfw {
	inline GLFWwindow * createWindow(const uvec2 & size, const ivec2 & position = ivec2(INT_MIN)) {
		GLFWwindow * window = glfwCreateWindow(size.x, size.y, "glfw", nullptr, nullptr);
		if (!window) {
			FAIL("Unable to create rendering window");
		}
		if ((position.x > INT_MIN) && (position.y > INT_MIN)) {
			glfwSetWindowPos(window, position.x, position.y);
		}
		return window;
	}
}

// A class to encapsulate using GLFW to handle input and render a scene
class GlfwApp {

protected:
	uvec2 windowSize;
	ivec2 windowPosition;
	GLFWwindow * window{ nullptr };
	unsigned int frame{ 0 };

public:
	GlfwApp();

	virtual ~GlfwApp();

	virtual int run();

protected:
	virtual GLFWwindow * createRenderingTarget(uvec2 & size, ivec2 & pos) = 0;

	virtual void draw() = 0;

	void preCreate();

	void postCreate();

	virtual void initGl();

	virtual void shutdownGl();

	virtual void finishFrame();

	virtual void destroyWindow();

	virtual void onKey(int key, int scancode, int action, int mods);
	 
	virtual void update();

	virtual void onMouseButton(int button, int action, int mods);

protected:
	virtual void viewport(const ivec2 & pos, const uvec2 & size);

private:

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	static void ErrorCallback(int error, const char* description);
};

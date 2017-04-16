#include "GLFWApp.h"
#include <iostream>

GlfwApp::GlfwApp() 
{
	// Initialize the GLFW system for creating and positioning windows
	if (!glfwInit()) {
		FAIL("Failed to initialize GLFW");
	}
	glfwSetErrorCallback(ErrorCallback);
}

GlfwApp::~GlfwApp() {
	if (nullptr != window) {
		glfwDestroyWindow(window);
	}
	glfwTerminate();
}
int GlfwApp::run() {
	preCreate();

	window = createRenderingTarget(windowSize, windowPosition);

	if (!window) {
		std::cout << "Unable to create OpenGL window" << std::endl;
		return -1;
	}

	postCreate();

	initGl();

	while (!glfwWindowShouldClose(window)) {
		++frame;
		glfwPollEvents();
		update();
		draw();
		finishFrame();
	}

	shutdownGl();

	return 0;
}
void GlfwApp::preCreate() {
	glfwWindowHint(GLFW_DEPTH_BITS, 16);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
}

void GlfwApp::postCreate() {
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwMakeContextCurrent(window);

	// Initialize the OpenGL bindings
	// For some reason we have to set this experminetal flag to properly
	// init GLEW if we use a core context.
	glewExperimental = GL_TRUE;
	if (0 != glewInit()) {
		FAIL("Failed to initialize GLEW");
	}

	glGetError();

	if (GLEW_KHR_debug) {
		GLint v;
		glGetIntegerv(GL_CONTEXT_FLAGS, &v);
		if (v & GL_CONTEXT_FLAG_DEBUG_BIT) {
			//glDebugMessageCallback(glDebugCallbackHandler, this);
		}
	}
}
void GlfwApp::initGl() {}

void GlfwApp::shutdownGl() {}

void GlfwApp::finishFrame() {
	glfwSwapBuffers(window);
}

void GlfwApp::destroyWindow() {
	glfwSetKeyCallback(window, nullptr);
	glfwSetMouseButtonCallback(window, nullptr);
	glfwDestroyWindow(window);
}

void GlfwApp::onKey(int key, int scancode, int action, int mods) {
	if (GLFW_PRESS != action) {
		return;
	}

	switch (key) {
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, 1);
		return;
	}
}

void GlfwApp::update() {}

void GlfwApp::onMouseButton(int button, int action, int mods) {}

void GlfwApp::viewport(const ivec2 & pos, const uvec2 & size) {
	glViewport(pos.x, pos.y, size.x, size.y);
}

void GlfwApp::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	GlfwApp * instance = (GlfwApp *)glfwGetWindowUserPointer(window);
	instance->onKey(key, scancode, action, mods);
}

void GlfwApp::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	GlfwApp * instance = (GlfwApp *)glfwGetWindowUserPointer(window);
	instance->onMouseButton(button, action, mods);
}

void GlfwApp::ErrorCallback(int error, const char* description) {
	FAIL(description);
}

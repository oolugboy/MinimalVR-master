#include "Project2App.h"
#define T_VERTEX_SHADER_PATH "./t_vertex_shader.vert"
#define T_FRAGMENT_SHADER_PATH "./t_fragment_shader.frag"

Project2App::Project2App()
{
	loadedShaders = false;
}
void Project2App::initGl(){
	RiftApp::initGl();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	ovr_RecenterTrackingOrigin(_session);

	/* Initialize the scene */
	cubeScene = new ColorCubeScene();
	/* Load the textures */
	cubeScene->loadTextures("vr_test_pattern.ppm");
}
void Project2App::shutdownGl() {
	delete(cubeScene);
}
void Project2App::update()
{
    /* only just spin the cube */
	cubeScene->update();
}
void Project2App::renderScene(const glm::mat4 & projection, const glm::mat4 & headPose) {

	/* Load the shader program */
	if (loadedShaders == false)
	{
		vrShaderProgram = LoadShaders(T_VERTEX_SHADER_PATH, T_FRAGMENT_SHADER_PATH);
		loadedShaders = true;
	}
	glUseProgram(vrShaderProgram);
	cubeScene->render(projection, glm::inverse(headPose), vrShaderProgram);	
}
Project2App::~Project2App()
{
}

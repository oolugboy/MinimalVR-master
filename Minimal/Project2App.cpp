#include "Project2App.h"
#define T_VERTEX_SHADER_PATH "./t_vertex_shader.vert"
#define T_FRAGMENT_SHADER_PATH "./t_fragment_shader.frag"

bool xPressed = false;
bool yPressed = false;
bool renderedFirst = false;

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
	updateControllersAction();
	
}
void Project2App::renderScene(const glm::mat4 & projection, const glm::mat4 & headPose) {

	glm::mat4 newHeadPose = headPose;

	/* Load the shader program */
	if (loadedShaders == false)
	{
		vrShaderProgram = LoadShaders(T_VERTEX_SHADER_PATH, T_FRAGMENT_SHADER_PATH);
		loadedShaders = true;
	}
	glUseProgram(vrShaderProgram);
	glm::mat4 view;
	if (getTrackIndex() == 0)
	{
		if (renderedFirst)
		{
			newHeadPose = oldHeadPose;			
		}
		else
		{
			renderedFirst = true;
		}		
	}
	if (getTrackIndex() == 1)
	{		
		newHeadPose[3] = oldHeadPose[3];
	}
	if (getTrackIndex() == 2)
	{
		newHeadPose[0] = oldHeadPose[0];
		newHeadPose[1] = oldHeadPose[1];
		newHeadPose[2] = oldHeadPose[2];
	}
	
	cubeScene->render(projection, glm::inverse(newHeadPose), vrShaderProgram);	

	oldHeadPose = newHeadPose;
}
void Project2App::updateControllersAction() {

	ovrInputState inputState;
	if (OVR_SUCCESS(ovr_GetInputState(_session, ovrControllerType_Touch, &inputState))) {
		
		if (inputState.Buttons == ovrTouch_X)
		{
			if(!xPressed)
			{
				nextDispIndex();
				xPressed = true;
			}
		}
		else
		{
			xPressed = false;
		}
		if (inputState.Buttons == ovrTouch_Y)
		{
			if (!yPressed)
			{
				nextTrackIndex();
				yPressed = true;
			}
		}
		else
		{
			yPressed = false;
		}
	}
}
Project2App::~Project2App()
{
}

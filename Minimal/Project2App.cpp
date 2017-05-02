#include <Windows.h>
#include "Project2App.h"

#define T_VERTEX_SHADER_PATH "./t_vertex_shader.vert"
#define T_FRAGMENT_SHADER_PATH "./t_fragment_shader.frag"

bool aPressed = false;
bool bPressed = false;
bool renderedRightFirst = false, renderedLeftFirst = false;
bool leftJoyEvent = false;

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
	updateControllersAction();
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
	
	glm::mat4 newHeadPose = headPose;
	glm::mat4 oldHeadPose;

	if (isRenderingLeft())
		oldHeadPose = oldLeftHeadPose;
	else
		oldHeadPose = oldRightHeadPose;

	if (getTrackIndex() == 0)
	{
		if(isRenderingLeft())
		{
			if (renderedLeftFirst)
			{
				newHeadPose = oldHeadPose;			
			}
			else
			{
				renderedLeftFirst = true;
			}	
		}	
		else
		{
			if (renderedRightFirst)
			{
				newHeadPose = oldHeadPose;
			}
			else
			{
				renderedRightFirst = true;
			}
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

	/* Check if rendering the left eye or right eye */
	if(isRenderingLeft())
		oldLeftHeadPose = newHeadPose;
	else
		oldRightHeadPose = newHeadPose;
}
void Project2App::updateControllersAction() {

	ovrInputState inputState;
	if (OVR_SUCCESS(ovr_GetInputState(_session, ovrControllerType_Touch, &inputState))) {
		
		if (inputState.Buttons == ovrTouch_A)
		{
			if(!aPressed)
			{
				nextDispIndex();
				aPressed = true;
			}
		}
		else
		{
			aPressed = false;
		}
		if (inputState.Buttons == ovrTouch_B)
		{
			if (!bPressed)
			{
				nextTrackIndex();
				bPressed = true;
			}
		}
		else
		{
			bPressed = false;
		}
		if (inputState.Thumbstick[ovrHand_Right].x < 0.000f)
		{
			decrIOD();
		}
		if (inputState.Thumbstick[ovrHand_Right].x > 0.000f)
		{
			incrIOD();
		}
		if (inputState.Buttons == ovrButton_RThumb)
		{
			resetIOD();
		}
		if (inputState.Thumbstick[ovrHand_Left].x < 0.000f)
		{
			/*if(!leftJoyEvent)
			{*/
				cubeScene->contractCube();
				leftJoyEvent = true;
			//}
		}
		else if (inputState.Thumbstick[ovrHand_Left].x > 0.000f)
		{
			/*if(!leftJoyEvent)
			{*/
				cubeScene->expandCube();
				leftJoyEvent = true;
			//}
		}
		else if (inputState.Buttons == ovrButton_LThumb)
		{
			/*if(!leftJoyEvent)
			{*/
				cubeScene->resetCubeSize();
				leftJoyEvent = true;
			//}
		}
		else
		{
			leftJoyEvent = false;
		}
	}
}
Project2App::~Project2App()
{
}

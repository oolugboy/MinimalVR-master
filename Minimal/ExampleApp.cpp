#include "ExampleApp.h"
#include <time.h>

#define VR_VERTEX_SHADER_PATH "./vr_vertex_shader.vert"
#define VR_FRAGMENT_SHADER_PATH "./vr_fragment_shader.frag"

bool shadersLoaded = false;

ExampleApp::ExampleApp()
{
	prevSpawnTime = currTime = prevTime = ((float)clock() / CLOCKS_PER_SEC);
}

void ExampleApp::initGl(){
	RiftApp::initGl();
	glClearColor(0.0f, 0.0f, 0.545f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	ovr_RecenterTrackingOrigin(_session);
	cubeScene = new ColorCubeScene();
	factoryScene = new FactoryScene();
}

void ExampleApp::shutdownGl(){
	delete(cubeScene);
}

void ExampleApp::renderScene(const glm::mat4 & projection, const glm::mat4 & headPose){

	float currTime = ((float)clock() / CLOCKS_PER_SEC);

	/* Load the shader program */
	if(shadersLoaded == false)
	{
		vrShaderProgram = LoadShaders(VR_VERTEX_SHADER_PATH, VR_FRAGMENT_SHADER_PATH);
		shadersLoaded = true;
	}
	glUseProgram(vrShaderProgram);
	//cubeScene->render(projection, glm::inverse(headPose), vrShaderProgram);

	//get ovr exact system time
	double displayMidpointSeconds = ovr_GetPredictedDisplayTime(_session, 0);

	//get current ovrState, contains info such as head position, hand positions
	ovrTrackingState trackState = ovr_GetTrackingState(_session, displayMidpointSeconds, ovrTrue);

	//two touch controller position, getting from trackState, each ovrPosef contains a vec4 orientation, vec3 position
	ovrPosef handPoses[2];
	ovrInputState inputState;

	// 0 is left, 1 is right
	handPoses[0] = trackState.HandPoses[ovrHand_Left].ThePose;
	handPoses[1] = trackState.HandPoses[ovrHand_Right].ThePose;

	//test left hand position
	bool leftHandHigh = false;
	if (handPoses[0].Position.y > 1.0f) {
		leftHandHigh = true;
	}

	bool leftHandTriggerPressed = true;

	if (OVR_SUCCESS(ovr_GetInputState(_session, ovrControllerType_Touch, &inputState))) {
		if (inputState.HandTrigger[ovrHand_Left] > 0.5f) {
			leftHandTriggerPressed = true;
		}
	}


	if (leftHandHigh && leftHandTriggerPressed) {
		ovr_SetControllerVibration(_session, ovrControllerType_LTouch, 0.0f, 1.0f);
	}
	else {
		ovr_SetControllerVibration(_session, ovrControllerType_LTouch, 0.0f, 0.0f);
	}

	/* Spawn a new molecule after 2 seconds */
	if (currTime - prevSpawnTime > 2.0f)
	{
		factoryScene->spawnNewMolecule();
		prevSpawnTime = currTime;
	}

	/* draw the factory scene */
	factoryScene->draw(vrShaderProgram, projection, glm::inverse(headPose), currTime - prevTime);
	prevTime = currTime;
}

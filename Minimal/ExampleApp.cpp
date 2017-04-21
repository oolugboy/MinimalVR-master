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
	bodyScene = new BodyScene();
}

void ExampleApp::shutdownGl(){
	delete(cubeScene);
}

void ExampleApp::update()
{
	updateControllersAction();
	
	if (rightHandTriggerPressed && leftHandTriggerPressed)
	{
		glm::vec3 lX1, rX1, lX2, rX2;
		bodyScene->getLeftLaserData(lX1, lX2);
		bodyScene->getRightLaserData(rX1, rX2);

		if (factoryScene->checkSlice(lX1, lX2, rX1, rX2)) {
			ovr_SetControllerVibration(_session, ovrControllerType_LTouch, 0.0f, 1.0f);
			ovr_SetControllerVibration(_session, ovrControllerType_RTouch, 0.0f, 1.0f);
		}
		else {
			ovr_SetControllerVibration(_session, ovrControllerType_LTouch, 0.0f, 0.0f);
			ovr_SetControllerVibration(_session, ovrControllerType_RTouch, 0.0f, 0.0f);
		}
	}
	else {
		ovr_SetControllerVibration(_session, ovrControllerType_LTouch, 0.0f, 0.0f);
		ovr_SetControllerVibration(_session, ovrControllerType_RTouch, 0.0f, 0.0f);
	}
	
	float currTime = ((float)clock() / CLOCKS_PER_SEC);

	/* Spawn a new molecule after 2 seconds */
	if (currTime - prevSpawnTime > 2.0f)
	{
		factoryScene->spawnNewMolecule();
		prevSpawnTime = currTime;
	}

	factoryScene->update(currTime - prevTime);
	prevTime = currTime;
}

void ExampleApp::renderScene(const glm::mat4 & projection, const glm::mat4 & headPose){

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
	ovrPosef handLPosef;
	ovrPosef handRPosef;

	ovrPosef headPosef = trackState.HeadPose.ThePose;

	handLPosef = trackState.HandPoses[ovrHand_Left].ThePose;
	handRPosef = trackState.HandPoses[ovrHand_Right].ThePose;
	
	//since hands are in same orientation with the head, we pass the position and later use it to translate the hand object
	glm::vec3 handLPos(handLPosef.Position.x, handLPosef.Position.y, handLPosef.Position.z);
	glm::vec3 handRPos(handRPosef.Position.x, handRPosef.Position.y, handRPosef.Position.z);

	//TODO:: for hand shooting, try to do Euler angle
	quat myQuatL(handLPosef.Orientation.w, handLPosef.Orientation.x, handLPosef.Orientation.y, handLPosef.Orientation.z);
	quat myQuatR(handRPosef.Orientation.w, handRPosef.Orientation.x, handRPosef.Orientation.y, handRPosef.Orientation.z);
	mat4 rotationL = glm::toMat4(myQuatL);
	mat4 rotationR = glm::toMat4(myQuatR);


	/* draw the factory scene */
	factoryScene->draw(vrShaderProgram, projection, glm::inverse(headPose));
	/*draw the body scene, hand and laser*/
	bodyScene->draw(vrShaderProgram, projection, glm::inverse(headPose), handLPos, handRPos, rotationL, rotationR, currTime - prevTime);
}

void ExampleApp::updateControllersAction() {

	ovrInputState inputState;
	if (OVR_SUCCESS(ovr_GetInputState(_session, ovrControllerType_Touch, &inputState))) {
		if (inputState.IndexTrigger[ovrHand_Left] > 0.5f) {
			//index trigger
			leftHandTriggerPressed = true;
			bodyScene->activateLeftLaser();
			//ovr_SetControllerVibration(_session, ovrControllerType_LTouch, 0.0f, 1.0f);
		}
		else {
			leftHandTriggerPressed = false;
			bodyScene->deActivateLeftLaser();
			//ovr_SetControllerVibration(_session, ovrControllerType_LTouch, 0.0f, 0.0f);
		}
		if (inputState.IndexTrigger[ovrHand_Right] > 0.5f) {
			//index trigger
			rightHandTriggerPressed = true;
			bodyScene->activateRightLaser();
			//ovr_SetControllerVibration(_session, ovrControllerType_RTouch, 0.0f, 1.0f);
		}
		else {
			rightHandTriggerPressed = false;
			bodyScene->deActivateRightLaser();
			//ovr_SetControllerVibration(_session, ovrControllerType_RTouch, 0.0f, 0.0f);
		}
		if (inputState.Buttons)
		{
			factoryScene->restartScene();
		}
	}
	
}

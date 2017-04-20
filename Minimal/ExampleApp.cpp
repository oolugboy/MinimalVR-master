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

void ExampleApp::update()
{
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
void ExampleApp::renderScene(const glm::mat4 & projection, const glm::mat4 & headPose)
{
	/* Load the shader program */
	if(shadersLoaded == false)
	{
		vrShaderProgram = LoadShaders(VR_VERTEX_SHADER_PATH, VR_FRAGMENT_SHADER_PATH);
		shadersLoaded = true;
	}
	glUseProgram(vrShaderProgram);
	//cubeScene->render(projection, glm::inverse(headPose), vrShaderProgram);

	/* draw the factory scene */
	factoryScene->draw(vrShaderProgram, projection, glm::inverse(headPose));
}

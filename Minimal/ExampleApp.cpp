#include "ExampleApp.h"

#define VR_VERTEX_SHADER_PATH "./vr_vertex_shader.vert"
#define VR_FRAGMENT_SHADER_PATH "./vr_fragment_shader.frag"

bool shadersLoaded = false;

ExampleApp::ExampleApp()
{

}

void ExampleApp::initGl(){
	RiftApp::initGl();
	glClearColor(0.0f, 0.0f, 0.545f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	ovr_RecenterTrackingOrigin(_session);
	cubeScene = new ColorCubeScene();
	factory = new Model("F:/CSE190Project/MinimalVR-master/ModelAssets/factory1/factory1.obj");
}

void ExampleApp::shutdownGl(){
	delete(cubeScene);
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
	factory->draw(vrShaderProgram, projection, glm::inverse(headPose));
}

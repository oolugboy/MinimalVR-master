#pragma once
#include "RiftApp.h"
#include "ColorCubeScene.h"
#include "Shaders.h"
#include "FactoryScene.h"
#include "BodyScene.h"

//////////////////////////////////////////////////////////////////////
//
// The remainder of this code is specific to the scene we want to 
// render.  I use oglplus to render an array of cubes, but your 
// application would perform whatever rendering you want
//
class ExampleApp : public RiftApp {
	ColorCubeScene * cubeScene;
	GLint vrShaderProgram;
	FactoryScene * factoryScene;
	BodyScene * bodyScene;
public:
	ExampleApp();

	protected:
	void initGl() override;
	void shutdownGl() override; 
	void update() override;
	void updateControllersAction();
	void updateControllersPosition();
	float prevSpawnTime;
	float prevTime;
	float currTime;
	void renderScene(const glm::mat4 & projection, const glm::mat4 & headPose) override;

private:
	bool leftHandTriggerPressed = false;
	bool rightHandTriggerPressed = false;
};
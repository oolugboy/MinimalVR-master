#pragma once
#include "RiftApp.h"
#include "ColorCubeScene.h"
#include "Shaders.h"
class Project2App :	public RiftApp
{
	ColorCubeScene * cubeScene;
	GLint vrShaderProgram;
	bool loadedShaders;
public:
	Project2App();
	~Project2App();
	void renderScene(const glm::mat4 & projection, const glm::mat4 & headPose) override;
	void updateControllersAction();	

protected:
	void initGl() override;
	void shutdownGl() override;
	void update() override;

	glm::mat4 oldLeftHeadPose;
	glm::mat4 oldRightHeadPose;
};


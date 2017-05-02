#pragma once
#include "GlfwApp.h"
#include "RiftManagerApp.h"

class RiftApp : public GlfwApp, public RiftManagerApp 
{
	public:
		void nextDispIndex();

		int getDispIndex();

		void nextTrackIndex();

		int getTrackIndex();

		bool isRenderingLeft();

		void decrIOD();

		void incrIOD();

		void resetIOD();

	private:
	GLuint _fbo{ 0 };
	GLuint _depthBuffer{ 0 };
	ovrTextureSwapChain _eyeTexture;

	GLuint _mirrorFbo{ 0 };
	ovrMirrorTexture _mirrorTexture;

	ovrEyeRenderDesc _eyeRenderDescs[2];

	mat4 _eyeProjections[2];

	ovrLayerEyeFov _sceneLayer;
	ovrViewScaleDesc _viewScaleDesc;

	uvec2 _renderTargetSize;
	uvec2 _mirrorSize;

	public:

	RiftApp();

	protected:
	GLFWwindow * createRenderingTarget(uvec2 & outSize, ivec2 & outPosition) override;

	void initGl() override;

	void onKey(int key, int scancode, int action, int mods) override;

	void draw() final override;

	virtual void renderScene(const glm::mat4 & projection, const glm::mat4 & headPose) = 0;

	~RiftApp();

	private:
		int dispIndex;
		int trackIndex;
		bool renderingLeft;
		ovrVector3f defLeftEyeOffset, defRightEyeOffset;
};



#include "RiftApp.h"
#include <algorithm>

namespace ovr {
	// Convenience method for looping over each eye with a lambda
	template <typename Function>
	inline void for_each_eye(Function function) {
		for (ovrEyeType eye = ovrEyeType::ovrEye_Left;
			eye < ovrEyeType::ovrEye_Count;
			eye = static_cast<ovrEyeType>(eye + 1)) {
			function(eye);
		}
	}

	inline mat4 toGlm(const ovrMatrix4f & om) {
		return glm::transpose(glm::make_mat4(&om.M[0][0]));
	}

	inline mat4 toGlm(const ovrFovPort & fovport, float nearPlane = 0.01f, float farPlane = 10000.0f) {
		return toGlm(ovrMatrix4f_Projection(fovport, nearPlane, farPlane, true));
	}

	inline vec3 toGlm(const ovrVector3f & ov) {
		return glm::make_vec3(&ov.x);
	}

	inline vec2 toGlm(const ovrVector2f & ov) {
		return glm::make_vec2(&ov.x);
	}

	inline uvec2 toGlm(const ovrSizei & ov) {
		return uvec2(ov.w, ov.h);
	}

	inline quat toGlm(const ovrQuatf & oq) {
		return glm::make_quat(&oq.x);
	}

	inline mat4 toGlm(const ovrPosef & op) {
		mat4 orientation = glm::mat4_cast(toGlm(op.Orientation));
		mat4 translation = glm::translate(mat4(), ovr::toGlm(op.Position));
		return translation * orientation;
	}

	inline ovrMatrix4f fromGlm(const mat4 & m) {
		ovrMatrix4f result;
		mat4 transposed(glm::transpose(m));
		memcpy(result.M, &(transposed[0][0]), sizeof(float) * 16);
		return result;
	}

	inline ovrVector3f fromGlm(const vec3 & v) {
		ovrVector3f result;
		result.x = v.x;
		result.y = v.y;
		result.z = v.z;
		return result;
	}

	inline ovrVector2f fromGlm(const vec2 & v) {
		ovrVector2f result;
		result.x = v.x;
		result.y = v.y;
		return result;
	}

	inline ovrSizei fromGlm(const uvec2 & v) {
		ovrSizei result;
		result.w = v.x;
		result.h = v.y;
		return result;
	}

	inline ovrQuatf fromGlm(const quat & q) {
		ovrQuatf result;
		result.x = q.x;
		result.y = q.y;
		result.z = q.z;
		result.w = q.w;
		return result;
	}
}
RiftApp::RiftApp()
{
	using namespace ovr;
	_viewScaleDesc.HmdSpaceToWorldScaleInMeters = 1.0f;

	memset(&_sceneLayer, 0, sizeof(ovrLayerEyeFov));
	_sceneLayer.Header.Type = ovrLayerType_EyeFov;
	_sceneLayer.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;

	ovr::for_each_eye([&](ovrEyeType eye) {
		ovrEyeRenderDesc& erd = _eyeRenderDescs[eye] = ovr_GetRenderDesc(_session, eye, _hmdDesc.DefaultEyeFov[eye]);
		ovrMatrix4f ovrPerspectiveProjection =
			ovrMatrix4f_Projection(erd.Fov, 0.01f, 1000.0f, ovrProjection_ClipRangeOpenGL);
		_eyeProjections[eye] = ovr::toGlm(ovrPerspectiveProjection);
		_viewScaleDesc.HmdToEyeOffset[eye] = erd.HmdToEyeOffset; // cse190 adjust the eye separation here 

		ovrFovPort & fov = _sceneLayer.Fov[eye] = _eyeRenderDescs[eye].Fov;
		auto eyeSize = ovr_GetFovTextureSize(_session, eye, fov, 1.0f);
		_sceneLayer.Viewport[eye].Size = eyeSize;
		_sceneLayer.Viewport[eye].Pos = { (int)_renderTargetSize.x, 0 };

		_renderTargetSize.y = std::max(_renderTargetSize.y, (uint32_t)eyeSize.h);
		_renderTargetSize.x += eyeSize.w;
	});
	// Make the on screen window 1/4 the resolution of the render target
	_mirrorSize = _renderTargetSize;
	_mirrorSize /= 4;
}
GLFWwindow * RiftApp::createRenderingTarget(uvec2 & outSize, ivec2 & outPosition)
{
	return glfw::createWindow(_mirrorSize);
}
void RiftApp::initGl(){
	GlfwApp::initGl();

	// Disable the v-sync for buffer swap
	glfwSwapInterval(0);

	ovrTextureSwapChainDesc desc = {};
	desc.Type = ovrTexture_2D;
	desc.ArraySize = 1;
	desc.Width = _renderTargetSize.x;
	desc.Height = _renderTargetSize.y;
	desc.MipLevels = 1;
	desc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.SampleCount = 1;
	desc.StaticImage = ovrFalse;
	ovrResult result = ovr_CreateTextureSwapChainGL(_session, &desc, &_eyeTexture);
	_sceneLayer.ColorTexture[0] = _eyeTexture;
	if (!OVR_SUCCESS(result)) {
		FAIL("Failed to create swap textures");
	}
	int length = 0;
	result = ovr_GetTextureSwapChainLength(_session, _eyeTexture, &length);
	if (!OVR_SUCCESS(result) || !length) {
		FAIL("Unable to count swap chain textures");
	}
	for (int i = 0; i < length; ++i) {
		GLuint chainTexId;
		ovr_GetTextureSwapChainBufferGL(_session, _eyeTexture, i, &chainTexId);
		glBindTexture(GL_TEXTURE_2D, chainTexId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	// Set up the framebuffer object
	glGenFramebuffers(1, &_fbo);
	glGenRenderbuffers(1, &_depthBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, _renderTargetSize.x, _renderTargetSize.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	ovrMirrorTextureDesc mirrorDesc;
	memset(&mirrorDesc, 0, sizeof(mirrorDesc));
	mirrorDesc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
	mirrorDesc.Width = _mirrorSize.x;
	mirrorDesc.Height = _mirrorSize.y;
	if (!OVR_SUCCESS(ovr_CreateMirrorTextureGL(_session, &mirrorDesc, &_mirrorTexture))) {
		FAIL("Could not create mirror texture");
	}
	glGenFramebuffers(1, &_mirrorFbo);
}
void RiftApp::onKey(int key, int scancode, int action, int mods){
	if (GLFW_PRESS == action) switch (key) {
	case GLFW_KEY_R:
		ovr_RecenterTrackingOrigin(_session);
		return;
	}

	GlfwApp::onKey(key, scancode, action, mods);
}
void RiftApp::draw()
{
	ovrPosef eyePoses[2];
	ovr_GetEyePoses(_session, frame, true, _viewScaleDesc.HmdToEyeOffset, eyePoses, &_sceneLayer.SensorSampleTime);

	int curIndex;
	ovr_GetTextureSwapChainCurrentIndex(_session, _eyeTexture, &curIndex);
	GLuint curTexId;
	ovr_GetTextureSwapChainBufferGL(_session, _eyeTexture, curIndex, &curTexId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, curTexId, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ovr::for_each_eye([&](ovrEyeType eye) {
		const auto& vp = _sceneLayer.Viewport[eye];
		glViewport(vp.Pos.x, vp.Pos.y, vp.Size.w, vp.Size.h);
		_sceneLayer.RenderPose[eye] = eyePoses[eye];
		renderScene(_eyeProjections[eye], ovr::toGlm(eyePoses[eye]));
//		renderScene(_eyeProjections[eye], ovr::toGlm(eyePoses[ovrEye_Left]));  // cse190: use eyePoses[ovrEye_Left] to render one eye's view to both eyes = monoscopic view
//		if (eye==ovrEye_Left) renderScene(_eyeProjections[eye], ovr::toGlm(eyePoses[eye]));  // cse190: this is how to render to only one eye
	});
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	ovr_CommitTextureSwapChain(_session, _eyeTexture);
	ovrLayerHeader* headerList = &_sceneLayer.Header;
	ovr_SubmitFrame(_session, frame, &_viewScaleDesc, &headerList, 1);

	GLuint mirrorTextureId;
	ovr_GetMirrorTextureBufferGL(_session, _mirrorTexture, &mirrorTextureId);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _mirrorFbo);
	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTextureId, 0);
	glBlitFramebuffer(0, 0, _mirrorSize.x, _mirrorSize.y, 0, _mirrorSize.y, _mirrorSize.x, 0, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}
/* Probably have to redo the update function
void update() final override
{
	ovrInputState inputState;
	if (OVR_SUCCESS(ovr_GetInputState(_session, ovrControllerType_Touch, &inputState)))
	{
		if (inputState.HandTrigger[ovrHand_Right] > 0.5f)   cerr << "right middle trigger pressed" << endl;
		if (inputState.IndexTrigger[ovrHand_Right] > 0.5f)	cerr << "right index trigger pressed" << endl;
		if (inputState.HandTrigger[ovrHand_Left] > 0.5f)    cerr << "left middle trigger pressed" << endl;
		if (inputState.IndexTrigger[ovrHand_Left] > 0.5f)	cerr << "left index trigger pressed" << endl;
		if (inputState.Buttons>0) cerr << "Botton state:" << inputState.Buttons << endl;
		// cse190: no need to print the above messages
	}
} */
RiftApp::~RiftApp()
{
}

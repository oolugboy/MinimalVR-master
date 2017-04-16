#pragma once
#include <OVR_CAPI.h>
#include <OVR_CAPI_GL.h>
#include <stdexcept>

#define FAIL(X) throw std::runtime_error(X)
class RiftManagerApp
{	
protected:
	ovrSession _session;
	ovrHmdDesc _hmdDesc;
	ovrGraphicsLuid _luid;

public:
	RiftManagerApp();
	~RiftManagerApp();
};


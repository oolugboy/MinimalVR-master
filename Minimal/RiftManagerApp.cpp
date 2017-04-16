#include "RiftManagerApp.h"



RiftManagerApp::RiftManagerApp()
{
	if (!OVR_SUCCESS(ovr_Create(&_session, &_luid))) {
		FAIL("Unable to create HMD session");
	}
	_hmdDesc = ovr_GetHmdDesc(_session);
}


RiftManagerApp::~RiftManagerApp()
{
	ovr_Destroy(_session);
	_session = nullptr;
}

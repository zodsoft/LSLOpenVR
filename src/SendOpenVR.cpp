#include "lsl_cpp.h"
#include "openvr.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>


std::string GetTrackedDeviceString(vr::IVRSystem *pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *peError = NULL)
{
	uint32_t unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, NULL, 0, peError);
	if (unRequiredBufferLen == 0)
		return "";

	char *pchBuffer = new char[unRequiredBufferLen];
	unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, pchBuffer, unRequiredBufferLen, peError);
	std::string sResult = pchBuffer;
	delete[] pchBuffer;
	return sResult;
}


void ProcessVREvent(const vr::VREvent_t & event)
{
	switch (event.eventType)
	{
	case vr::VREvent_TrackedDeviceActivated:
	{
		//SetupRenderModelForTrackedDevice(event.trackedDeviceIndex);
		std::cout << "Attached device" << event.trackedDeviceIndex << std::endl;
	}
	break;
	case vr::VREvent_TrackedDeviceDeactivated:
	{
		std::cout << "Detached device " << event.trackedDeviceIndex << std::endl;
	}
	break;
	case vr::VREvent_TrackedDeviceUpdated:
	{
		std::cout << "Updated device " << event.trackedDeviceIndex << std::endl;
	}
	break;
	}
}


int main(int argc, char* argv[]) {
    std::string pollControllers;
    //TODO: Input argument to specify whether or not to use controllers.
    if (argc > 1) {
        pollControllers = argv[1];
    }
	//TODO: Parse pollControllers to a boolean

    try {

        // TODO: Open libopenvr, get device parameters
        // Need nChannels and maybe poll a few samples to get sampRate
        // Device name
		vr::EVRInitError eError = vr::VRInitError_None;
		vr::IVRSystem *vrsys = vr::VR_Init(&eError, vr::VRApplication_Other);
		if (eError != vr::VRInitError_None)
		{
			vrsys = NULL;
			char buf[1024];
			sprintf_s(buf, sizeof(buf), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
			std::cout << buf << std::endl;
			return false;
		}

		std::string strDriver = "No Driver";
		std::string strDisplay = "No Display";

		strDriver = GetTrackedDeviceString(vrsys, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String);
		strDisplay = GetTrackedDeviceString(vrsys, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String);
		std::cout << "Driver: " << strDriver << "; Display: " << strDisplay << std::endl;
		//Driver: lighthouse; Display: LHR-5D35FA67

		bool deviceIsTracked[vr::k_unMaxTrackedDeviceCount];

		/*
		vr::TrackingUniverseOrigin eOrigin = vr::TrackingUniverseSeated;
		vr::TrackedDevicePose_t *pTrackedDevicePoseArray;
		uint32_t unTrackedDevicePoseArrayCount = 1;
		*/
        
        /*
        // TODO: make 2 new stream_info. One for Button events, one for Position
        lsl::stream_info infoButtons("OpenVRButtons", "Markers", 1, lsl::IRREGULAR_RATE, lsl::cf_string, "OpenVRButtons_");
        //TODO: Add XML info
        //TODO: Get manufacturer from libopenvr
        //info.desc().append_child_value("manufacturer", "HTC Vive");
        lsl::stream_outlet outletButtons(infoButtons);
        
        lsl::stream_info infoPoses("OpenVRPoses", "Position", 36, 60, lsl::cf_float32, "OpenVRPoses_");
        // TODO: Add XML info
        //info.desc().append_child_value("manufacturer", "HTC Vive");
        //lsl::xml_element channels = info.desc().append_child("channels");
        //channels.append_child("channel")
        //  .append_child_value("label","X")
        //  .append_child_value("type","PositionX")
        //  .append_child_value("unit","normalized_signed");
        lsl::stream_outlet outletPoses(infoPoses);
        */
        
        // send data forever
        std::cout << "Now sending data... " << std::endl;
        
        for(unsigned t=0;;t++) {

			// Process SteamVR events
			vr::VREvent_t event;
			while (vrsys->PollNextEvent(&event, sizeof(event)))
			{
				ProcessVREvent(event);
			}

			// Process SteamVR controller state
			for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++)
			{
				vr::VRControllerState_t state;
				if (vrsys->GetControllerState(unDevice, &state))
				{
					deviceIsTracked[unDevice] = state.ulButtonPressed == 0;
				}
			}
            
            //TODO: Get button events from openvr
            //TODO: If there were button events, then push a sample through outletButtons
			//vrsys->GetDeviceToAbsoluteTrackingPose(eOrigin, 0.0f, VR_ARRAY_COUNT(unTrackedDevicePoseArrayCount) pTrackedDevicePoseArray, unTrackedDevicePoseArrayCount);
            
            //TODO: Get the state and pose from openvr
            //Always push the latest sample
            /*
            float sample[36] = {js.lX,js.lY,js.lZ,js.lRx,js.lRy,js.lRz,js.rglSlider[0],js.rglSlider[1],
                js.rgdwPOV[0],js.rgdwPOV[1],js.rgdwPOV[2],js.rgdwPOV[3],js.lVX,js.lVY,js.lVZ,js.lVRx,js.lVRy,js.lVRz,
                js.rglVSlider[0],js.rglVSlider[1],js.lAX,js.lAY,js.lAZ,js.lARx,js.lARy,js.lARz,js.rglASlider[0],
                js.rglASlider[1],js.lFX,js.lFY,js.lFZ,js.lFRx,js.lFRy,js.lFRz,js.rglFSlider[0],js.rglFSlider[1]};
            outletPoses.push_sample(sample);
            */
        }

    } catch(std::exception &e) {
        std::cerr << "Got an exception: " << e.what() << std::endl;
		vr::VR_Shutdown();
		//vrsys = NULL;
    }
    std::cout << "Press any key to exit. " << std::endl; std::cin.get();

	
    return 0;
}

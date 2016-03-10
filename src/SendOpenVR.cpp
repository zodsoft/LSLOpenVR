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

void
PrintPose(vr::HmdMatrix34_t pose_mat)
{
	double clk = lsl::local_clock();
	std::cout
		<< fmod(1000.0*clk, 1000)
		<< ", "
		<< pose_mat.m[0][0]
		<< ", "
		<< pose_mat.m[0][1]
		<< ", "
		<< pose_mat.m[0][2]
		<< ", "
		<< pose_mat.m[0][3]
		<< ", "
		<< pose_mat.m[1][0]
		<< ", "
		<< pose_mat.m[1][1]
		<< ", "
		<< pose_mat.m[1][2]
		<< ", "
		<< pose_mat.m[1][3]
		<< ", "
		<< pose_mat.m[2][0]
		<< ", "
		<< pose_mat.m[2][1]
		<< ", "
		<< pose_mat.m[2][2]
		<< ", "
		<< pose_mat.m[2][3]
		<< std::endl;
};

const int n_devices = 3;
std::vector<std::string> device_chans = {
	"HMD",
	"Left",
	"Right"
};
const int n_pose_axes = 3 * 4;
std::vector<std::string> pose_chans = {
	"00", "01", "02", "03",
	"10", "11", "12", "13",
	"20", "21", "22", "23"
};


int main(int argc, char* argv[]) {
    std::string pollControllers;
    //TODO: Input argument to specify whether or not to use controllers.
    if (argc > 1) {
        pollControllers = argv[1];
    }
	//TODO: Parse pollControllers to a boolean

    try {
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

		std::string manufac = GetTrackedDeviceString(vrsys, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_ManufacturerName_String);
		std::cout
			<< "Manufacturer: " << manufac
			<< "; Driver: " << GetTrackedDeviceString(vrsys, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String)
			<< "; Display: " << GetTrackedDeviceString(vrsys, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String)
			<< std::endl;
		//Manufacturer: HTC; Driver: lighthouse; Display: LHR-5D35FA67

		if (vrsys->IsInputFocusCapturedByAnotherProcess())
		{
			std::cout << "Input focus is captured by another process" << std::endl;
		}

		// Wait until all devices are tracking

		std::vector<uint32_t> device_indices = {
			vr::k_unTrackedDeviceIndex_Hmd,
			(uint32_t)vrsys->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand),
			(uint32_t)vrsys->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand)
		};
		std::cout << device_indices[0] << " " << device_indices[1] << " " << device_indices[2] << std::endl;
		
		//Make stream infos. First for Button events. Second for poses.
        lsl::stream_info infoButtons("Buttons", "Markers", 1, lsl::IRREGULAR_RATE, lsl::cf_int8, "OpenVRButtons_");
		infoButtons.desc().append_child_value("manufacturer", manufac);
		lsl::xml_element butt_channels = infoButtons.desc().append_child("channels");
		butt_channels.append_child("channel")
			.append_child_value("label", "Controllers")
			.append_child_value("type", "ButtonCode")
			.append_child_value("unit", "uint8_code");
        lsl::stream_outlet outletButtons(infoButtons);
        		
        lsl::stream_info infoPoses("OpenVRPoses", "Position", n_devices*n_pose_axes, 300, lsl::cf_float32, "OpenVRPoses_");
		infoPoses.desc().append_child_value("manufacturer", manufac);
        lsl::xml_element pose_channels = infoPoses.desc().append_child("channels");
		int device_id;  // Index into our list of 3 devices: HMD, Left, Right
		int pose_ax;
		for (device_id = 0; device_id < n_devices; device_id++)
		{
			for (pose_ax = 0; pose_ax < n_pose_axes; pose_ax++)
			{
				std::string ch_label = device_chans[device_id] + pose_chans[pose_ax];
				pose_channels.append_child("channel")
					.append_child_value("label", ch_label)
					.append_child_value("type", "Pose")
					.append_child_value("unit", "m");
			}
		}
        lsl::stream_outlet outletPoses(infoPoses);
        
        // send data forever
        std::cout << "Now sending data... " << std::endl;
        for(unsigned t=0;;t++) {

			// Process SteamVR events (i.e., button presses)
			vr::VREvent_t event;
			while (vrsys->PollNextEvent(&event, sizeof(event)))
			{
				vr::TrackedDeviceIndex_t devId = event.trackedDeviceIndex;
				vr::ETrackedDeviceClass devClass = vrsys->GetTrackedDeviceClass(devId);  // TrackedDeviceClass_Controller = 2
				if (devClass == vr::TrackedDeviceClass_Controller)
				{
					// TrackedControllerRole_LeftHand or TrackedControllerRole_RightHand
					vr::ETrackedControllerRole ctrlRole = vrsys->GetControllerRoleForTrackedDeviceIndex(devId);

					//0 = k_EButton_System; 1 = k_EButton_ApplicationMenu; 2 = k_EButton_Grip (either); 32 = k_EButton_Axis0 = k_EButton_SteamVR_Touchpad; 33 = k_EButton_Axis1 = k_EButton_SteamVR_Trigger
					vr::EVRButtonId butId = (vr::EVRButtonId)event.data.controller.button;

					//VREvent_TrackedDeviceActivated = 100; VREvent_TrackedDeviceUserInteractionStarted = 103; VREvent_TrackedDeviceUserInteractionEnded = 104; VREvent_ButtonPress = 200; VREvent_ButtonUnpress = 201; VREvent_ButtonTouch = 202; VREvent_ButtonUntouch = 203;
					vr::EVREventType evType = (vr::EVREventType)event.eventType;

					// We are interested in 2 controllers. Use 1-2 MSB (markerVal >> 7)
					// We are interested in 4 buttons. Use 3-5 MSB ((markerVal % 128) >> 3)
					// We are interested in 7 events. Use 6-8 MSB (markerVal % 8)
					uint8_t markerVal = 0;
					if (ctrlRole == vr::TrackedControllerRole_LeftHand)
					{
						markerVal |= (1 << 7);
					}
					
					uint32_t button = butId;
					if (button >= 32)
					{
						button -= 29;
					}
					markerVal |= button << 3;

					uint32_t event = evType;
					event -= 100;
					if (event >= 3)
					{
						event -= 2;
					}
					if (event >= 98)
					{
						event -= 95;
					}
					markerVal |= event;

					const int sample = markerVal;
					outletButtons.push_sample(&sample);
					
					std::cout
						<< ((ctrlRole == vr::TrackedControllerRole_LeftHand) ? "Left" : "Right")
						<< "; Button: " << vrsys->GetButtonIdNameFromEnum(butId)
						<< "; Event: " << vrsys->GetEventTypeNameFromEnum(evType)
						<< "; Marker Val: " << static_cast<int16_t>(markerVal)
						<< std::endl;
				}
			}

			// Get the poses
			vr::TrackedDevicePose_t rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
			float pose_sample[n_devices*n_pose_axes];
			vrsys->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseSeated, 0.0f, VR_ARRAY_COUNT(vr::k_unMaxTrackedDeviceCount) rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount);
			for (int device_ix = 0; device_ix < vr::k_unMaxTrackedDeviceCount; device_ix++)
			{
				if (rTrackedDevicePose[device_ix].bPoseIsValid)
				{
					vr::HmdMatrix34_t pose_mat = rTrackedDevicePose[device_ix].mDeviceToAbsoluteTracking;

					if (device_ix == vr::k_unTrackedDeviceIndex_Hmd)
					{
						device_id = 0;
					}
					else if (device_ix == vrsys->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand))
					{
						device_id = 1;
					}
					else if (device_ix == vrsys->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand))
					{
						device_id = 2;
					}

					if (device_id == 99)
					{
						PrintPose(pose_mat);
					}
					
					for (int row_ix = 0; row_ix < 3; row_ix++)
					{
						for (int col_ix = 0; col_ix < 4; col_ix++)
						{
							int sample_ix = (3 * 4 * device_id) + (4 * row_ix) + col_ix;
							pose_sample[sample_ix] = pose_mat.m[row_ix][col_ix];
						}
					}
					outletPoses.push_sample(pose_sample);
				}
			}	
        }

    } catch(std::exception &e) {
        std::cerr << "Got an exception: " << e.what() << std::endl;
		vr::VR_Shutdown();
		//vrsys = NULL;
    }
    std::cout << "Press any key to exit. " << std::endl; std::cin.get();

	
    return 0;
}

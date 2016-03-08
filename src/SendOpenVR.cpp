#include "lsl_cpp.h"

#include <iostream>
#include <time.h>
#include <stdlib.h>


int main(int argc, char* argv[]) {
    std::string pollControllers;
    //TODO: Input argument to specify whether or not to use controllers.
    if (argc > 1) {
        pollControllers = argv[1];
    }

    try {

        // TODO: Open libopenvr, get device parameters
        // Need nChannels and maybe poll a few samples to get sampRate
        // Device name
        
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
            
            //TODO: Get button events from openvr
            //TODO: If there were button events, then push a sample through outletButtons
            
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
    }
    std::cout << "Press any key to exit. " << std::endl; cin.get();
    return 0;
}

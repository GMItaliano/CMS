#include "livestream_ctrl.h"

livestream_ctrl::livestream_ctrl(int cam) : camera(0)
{

    Cdevice = cam;
    //Mdevice = micro;

    camera = camera_sys(Cdevice);
    //initialize microphone

}

livestream_ctrl::~livestream_ctrl(){

}

void livestream_ctrl::start_livestream(){
    
    live_flag = 1;

    camera.record_video(live_flag);
    micro.record_audio(live_flag);
    

    //send to NGINX
}

void livestream_ctrl::stop_livestream(){
    
    live_flag = 0;

    camera.record_video(live_flag);
    micro.record_audio(live_flag);

    //stop sending to NGINX
}
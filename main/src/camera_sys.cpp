#include "camera_sys.h"

using namespace cv;

//Base values
#define FRAMES 30
#define WIDTH 1220
#define HEIGHT 780


//Constructor & Destructor

camera_sys::camera_sys(int device = 0){

    this -> device = device;
    stream_flag = 1;

    cap.open(device);
    if(!cap.isOpened())
        std::cerr << "ERROR: CANNOT OPEN DEVICE! DEVICE ID: "<< device << std::endl;

    cap.set(CAP_PROP_FRAME_WIDTH, WIDTH);
    cap.set(CAP_PROP_FRAME_HEIGHT, HEIGHT);
    cap.set(CAP_PROP_FPS, FRAMES);

}

camera_sys::~camera_sys(){
    cap.release();
}

//main Functions

void camera_sys::record_video(bool ctrl){

    stream_flag = ctrl;

    if(cap.isOpened() && stream_flag){
        while(1){
        if(cap.read(frame)){
            imshow("Live video: ", frame);
        }else
            std::cerr << "ERROR: CANNOT GET FRAME" << std::endl;
        }
    }else{
        std::cerr << "ERROR: CANNOT OPEN DEVICE " << device << std::endl;
        destroyAllWindows();
    }


}

void camera_sys::configure(int fr, int qual){

    fr_count = fr;

    switch (qual)
    {
    case 360:
        frw = 640;
        frh = 360;
        break;
    case 480:
        frw = 720;
        frh = 480;
        break;
    case 720:
        frw = 1080;
        frh = 720;
        break;
    case 1080:
        frw = 1920;
        frh = 1080;
        break;
    default:
        frw = WIDTH;
        frh = HEIGHT;
        break;
    }

    cap.set(CAP_PROP_FRAME_WIDTH, frw);
    cap.set(CAP_PROP_FRAME_HEIGHT, frh);
    cap.set(CAP_PROP_FPS, fr);

}
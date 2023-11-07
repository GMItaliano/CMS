#ifndef __CAMERA_SYS_H__
#define __CAMERA_SYS_H__

#include <unistd.h>
#include <ctime>
#include <string>

#include<opencv2/opencv.hpp>//OpenCV header to use VideoCapture class//

using namespace cv;

//---- DEVICES ----
// 0 -> PC camera
// X -> RASP (Don't know yet)

class camera_sys{

    private:
        bool stream_flag;
        int frw, frh;
        int fr_count;

        std::string t_now;

        int device;
        Mat frame;
        VideoCapture cap;

    public:
        camera_sys(int);
        ~camera_sys();

        void record_video(bool);
        void configure(int, int);

};

#endif __CAMERA_SYS_H__
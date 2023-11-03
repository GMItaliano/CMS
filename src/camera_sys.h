#ifndef __CAMERA_SYS_H__
#define __CAMERA_SYS_H__

#include <unistd.h>
#include <ctime>
#include <string>

class camera_sys{

    private:
        bool stream_flag;
        int frw, frh;
        int fr_count;

        std::string t_now;


    public:
        camera_sys();
        ~camera_sys();

        void record_video();
        void configure(int, int);

};

#endif __CAMERA_SYS_H__
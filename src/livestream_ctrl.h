#ifndef LIVESTREAM_CTRL_H
#define LIVESTREAM_CTRL_H

#include <string>
#include <iostream>

#include "camera_sys.h"
#include "speaker_sys.h"

class livestream_ctrl{
    private:
        std::string path;
        std::string database_link;
        bool live_flag;

        camera_sys camera;
        speaker_sys speaker;

    public:

        livestream_ctrl();
        ~livestream_ctrl();

        void start_livestream();
        void stop_livestream();


};


#endif LIVESTREAM_CTRL_H
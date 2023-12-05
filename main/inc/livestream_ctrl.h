#ifndef LIVESTREAM_CTRL_H
#define LIVESTREAM_CTRL_H

#include <string>
#include <iostream>

#include "camera_sys.h"
#include "microphone_sys.h"

class livestream_ctrl{ 
    private:
        std::string path;
        std::string database_link;
        bool live_flag;

        int Cdevice;
       // int Mdevice;

        camera_sys camera;
        microphone_sys micro;

    public:

        livestream_ctrl(int);
        ~livestream_ctrl();

        void start_livestream();
        void stop_livestream();


};


#endif 
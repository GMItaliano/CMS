#ifndef LIVESTREAM_CTRL_H
#define LIVESTREAM_CTRL_H

#include <string>
#include <iostream>

class livestream_ctrl{ 
    private:
        // std::string path;
        // std::string database_link;
        // bool live_flag;
        // int Cdevice;
        // int Mdevice;

    public:

        livestream_ctrl();
        ~livestream_ctrl();

        void start_livestream();
        void stop_livestream();
        void play_audio();


};


#endif 
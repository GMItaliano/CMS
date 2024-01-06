#ifndef DATABASE_SYS_H
#define DATABASE_SYS_H

#include <string>
#include <iostream>
//#include <python3.10/Python.h>
#include </home/goncalo/buildroots/buildroot-2021.08-Fato/output/host/include/python3.9/Python.h>
#include <filesystem>
//#include "house_system.h"


class database_sys{
    private:
        
        PyObject *pModule; 
        //PyObject *fsendData, *freceiveData, *finnitDatabase, *fresult, *fupdatefData;
        //std::string pID; 
    
    public:

        database_sys();
        ~database_sys();

        void send_data(const std::string& path, const std::string& key, const std::string& value);
        void push_data(const std::string& path, const std::string& data);
        std::string receive_data(const std::string& path);
        void flags_update(int type, bool state);
        void download_audio();
        //const char* read_flags();
        //void update_logs()
};

#endif
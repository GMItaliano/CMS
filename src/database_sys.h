#ifndef DATABASE_SYS_H
#define DATABASE_SYS_H

#include <string>

class database_sys{
    private:
        std::string path;
        std::string link_database;
    
    public:

        database_sys();
        ~database_sys();

        void send_data(std::string, std::string);
        void receive_data(std::string);

        void flags_update(int, bool);           //not final only an idea

};

#endif DATABASE_SYS_H
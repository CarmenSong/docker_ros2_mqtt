#include <filesystem>
#include <chrono>
#include <filesystem>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include "unistd.h"

#define Lib_PATH "/home/jessewl/work/C-Programs/gRPC-mqtt/build"

std::vector<std::string> Lib_container {"/home/jessewl/work/C-Programs/gRPC-mqtt/build/libdlopenRos.so"};
bool dirc_update = false;
bool roslib_update = false;

std::string get_dirctime(std::filesystem::file_time_type tp)
{
        auto sec = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch());
        std::time_t t = sec.count();
        const std::tm* lt = std::localtime(&t);
        const std::string get_localtime = static_cast<std::ostringstream&&>(std::ostringstream() << std::put_time(lt, "%c")).str();

        return get_localtime;
        
}

bool if_dirc_update(std::string initial_filetime, std::filesystem::file_time_type filetime)
{


        filetime = std::filesystem::last_write_time(Lib_PATH);
        std::string update_date = get_dirctime(filetime);
        if (update_date != initial_filetime)  {
                std::cout << "Dirc Updated" << std::endl;

                return true;
        };
        sleep(1);

        return false;
}

void lib_management() {
        for (const std::filesystem::directory_entry& x : std::filesystem::directory_iterator(Lib_PATH))
        {
                if (x.path().extension() == ".so" && std::find(Lib_container.begin(), Lib_container.end(), x.path()) == Lib_container.end()) {

                        Lib_container.push_back(x.path());
                        //std::cout << "New Lib Prepared" << std::endl;
                        roslib_update = true;
                } 
        }
        //return false;
}
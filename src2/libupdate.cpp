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

#define Lib_PATH "/home/jessewl/work/C-Programs/gRPC-mqtt/"

namespace mesg
{
        std::vector<std::string> Lib_Container {"/home/jessewl/work/C-Programs/gRPC-mqtt/build/libdlopenRos.so"};
        bool dirc_update = false;
        bool roslib_update = false;

        std::string get_dirctime (std::filesystem::file_time_type dirc_time)
        {
                auto sec = std::chrono::duration_cast<std::chrono::seconds>(dirc_time.time_since_epoch());
                std::time_t t = sec.count();
                const std::tm* lt = std::localtime(&t); 
                const std::string get_localtime = static_cast<std::ostringstream&&>(std::ostringstream() << std::put_time(lt, "%c")).str();

                return get_localtime;
        }
}

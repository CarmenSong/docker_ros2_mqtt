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

namespace mesg
{
        std::vector<std::string> Lib_Container{"/home/jessewl/work/C-Programs/gRPC-mqtt/build/libdlopenRos.so"};
        bool dirc_update = false;
        bool roslib_update = false;

        std::string get_dirctime(std::filesystem::file_time_type dirc_time)
        //check to modified timestamp of file
        {
                auto sec = std::chrono::duration_cast<std::chrono::seconds>(dirc_time.time_since_epoch());
                std::time_t t = sec.count();
                const std::tm *lt = std::localtime(&t);
                const std::string get_localtime = static_cast<std::ostringstream &&>(std::ostringstream() << std::put_time(lt, "%c")).str();

                return get_localtime;
        }

        bool if_dirc_update(std::string initial_filetime, std::filesystem::file_time_type filetime)
        // check whether directory is updated
        {
                filetime = std::filesystem::last_write_time(Lib_PATH);
                std::string last_update_time = get_dirctime(filetime);

                if (last_update_time != initial_filetime)
                {
                        std::cout << "Dirc Updated" << std::endl;
                        // initial_filetime = last_update_time;
                        return true;
                }
                return false;
        }

        void lib_management()
        // check whether some new D-libs were updated
        {
                for (const std::filesystem::directory_entry &x : std::filesystem::directory_iterator(Lib_PATH))
                {
                        if (x.path().extension() == ".so" && std::find(Lib_Container.begin(), Lib_Container.end(), x.path()) == Lib_Container.end())
                        {
                                Lib_Container.push_back(x.path());
                                roslib_update = true;
                        }
                }
        }
}

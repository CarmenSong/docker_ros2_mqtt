#include <iostream>
#include <filesystem>
#include <chrono>
#include <filesystem>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <thread>
#include <memory>
#include <list>

#include "dlfcn.h"
#include "unistd.h"
#include "libupdate.cpp"
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "LogHandlerPub.h"
#include "LogHandlerSub.h"

extern std::string received_msg;

static std::filesystem::file_time_type dirc_time = std::filesystem::last_write_time(Lib_PATH);

static std::string initial_dirctime = get_dirctime(dirc_time);
// bool iflibupdate = false;
mesg::LogHandlerPub handle_pub;
mesg::LogHandlerSub handle_sub;

extern std::string received_msg;

namespace mesg
{
        void check_lib_update(std::string initial_dirctime, std::filesystem::file_time_type ft)
        // lib更新されたかをチェック
        {
                while (received_msg != "3")
                {
                        bool dirc_update = if_dirc_update(initial_dirctime, ft); // check whether directory is updated
                        if (dirc_update)
                        {
                                ft = std::filesystem::last_write_time(Lib_PATH);
                                initial_dirctime = get_dirctime(ft); // initial dirctimeを更新
                                lib_management();                    // Lib_Containerにlibを追加

                                handle_pub.init();
                                handle_pub.log_pub_message("dynamic-lib update success, Please press 4 to reload it");
                        }
                }
        }

        void updatedlib_precheck()
        {
                char *lib_path = const_cast<char *>(Lib_container[Lib_container.size() - 1].c_str());
                const auto dlopenRos = dlopen(lib_path, RTLD_NOW);
                if (dlopenRos == NULL)
                {
                        std::cout << "dlopen failed" << dlerror() << std::endl;
                        return;
                }
                else
                {
                        handle_pub.init();
                        handle_pub.log_pub_message("PreCheck: dlopen success!");
                        sleep(1);
                }
                auto Moveturtle = reinterpret_cast<void (*)()>(dlsym(dlopenRos, "_Z9RunTurtlev"));
                // auto Moveturtle = reinterpret_cast<void (*)()>(dlsym(dlopenRos, "_Z7RunTestv"));

                if (Moveturtle == NULL)
                {
                        std::cout << "dlsym failed" << dlerror() << std::endl;
                        return;
                }
                else
                {
                        handle_pub.init();
                        handle_pub.log_pub_message("Pre_Check: dlsym success!");
                        sleep(1);
                }

                dlclose(dlopenRos);
        }

        void ifmsgequal5()
        {
                while (received_msg != "3")
                {
                        if (received_msg == "5")
                        {
                                updatedlib_precheck();
                                break;
                        }
                        else if (received_msg != "1")
                        {
                                break;
                        }
                }
        }

        void loading_turtlesim_lib()
        {
                // std::thread rcvdecision_thrd([&](){handle_sub.rcvdecision_init();});

                char *lib_path = const_cast<char *>(Lib_container[Lib_container.size() - 1].c_str());
                const auto dlopenRos = dlopen(lib_path, RTLD_NOW);
                if (dlopenRos == NULL)
                {
                        std::cout << "dlopen failed" << dlerror() << std::endl;
                        return;
                }
                else
                {
                        handle_pub.init();
                        handle_pub.log_pub_message("dlopen success!");
                        sleep(1);
                }
                auto Moveturtle = reinterpret_cast<void (*)()>(dlsym(dlopenRos, "_Z9RunTurtlev"));
                // auto Moveturtle = reinterpret_cast<void (*)()>(dlsym(dlopenRos, "_Z7RunTestv"));

                if (Moveturtle == NULL)
                {
                        std::cout << "dlsym failed" << dlerror() << std::endl;
                        return;
                }
                else
                {
                        handle_pub.init();
                        handle_pub.log_pub_message("dlsym success!");
                        sleep(1);
                }

                Moveturtle(); // roop
                dlclose(dlopenRos);
                // rcvdecision_thrd.join();
        }
}

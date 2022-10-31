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

using std::this_thread::sleep_for;

extern std::string received_msg;

static std::filesystem::file_time_type dirc_time = std::filesystem::last_write_time(Lib_PATH);
static std::string initial_dirctime = get_dirctime(dirc_time);
// bool iflibupdate = false;
mesg::LogHandlerPub handle_pub;
mesg::LogHandlerSub handle_sub;

mesg::stateofturtle_pub turtlestate;

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
                                turtlestate.stateofturtle_pub_init();
                                turtlestate.stateofturtle_pubmsg("STATE: POWER ON [UPDATING]");
                                ft = std::filesystem::last_write_time(Lib_PATH);
                                initial_dirctime = get_dirctime(ft); // initial dirctimeを更新
                                lib_management();                    // Lib_Containerにlibを追加
                                sleep(1);
                                // handle_pub.init();
                                // handle_pub.log_pub_message("dynamic-lib update success, press 5 to Pre-Check");
                                turtlestate.stateofturtle_pub_init();
                                turtlestate.stateofturtle_pubmsg("STATE: POWER ON [UPDATE COMPLETE]");
                                sleep(1);
                                if (received_msg == "1")
                                {
                                        turtlestate.stateofturtle_pub_init();
                                        turtlestate.stateofturtle_pubmsg("STATE: POWER ON [AUTOMATIC]");
                                }
                                
                        }
                }
        }

        void updatedlib_precheck()
        {
                turtlestate.stateofturtle_pub_init();
                turtlestate.stateofturtle_pubmsg("STATE: POWER ON [PRECHECKING]");
                sleep(2);

                char *lib_path = const_cast<char *>(Lib_container[Lib_container.size() - 1].c_str());
                const auto dlopenRos = dlopen(lib_path, RTLD_NOW);
                if (dlopenRos == NULL)
                {
                        std::cout << "dlopen failed" << dlerror() << std::endl;
                        return;
                }

                auto Moveturtle = reinterpret_cast<void (*)()>(dlsym(dlopenRos, "_Z18RunTurtle_precheckv"));
                // auto Moveturtle = reinterpret_cast<void (*)()>(dlsym(dlopenRos, "_Z7RunTestv"));

                if (Moveturtle == NULL)
                {
                        std::cout << "dlsym failed" << dlerror() << std::endl;
                        return;
                }
                else
                {
                        handle_pub.init();
                        handle_pub.log_pub_message("Precheck Success! Press 4 to reload");
                        sleep_for(std::chrono::milliseconds(500));
                }
                Moveturtle();
                dlclose(dlopenRos);
                return;
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
                return;
        }

        void loading_turtlesim_lib()
        {
                // std::thread rcvdecision_thrd([&](){handle_sub.rcvdecision_init();});
                char *lib_path = const_cast<char *>(Lib_container[Lib_container.size() - 1].c_str());
                const auto dlopenRos = dlopen(lib_path, RTLD_NOW);
                if (dlopenRos == NULL)
                {
                        std::cout << "dlopen failed" << dlerror() << std::endl;
                        handle_pub.init();
                        handle_pub.log_pub_message(dlerror());
                        return;
                }
                auto Moveturtle = reinterpret_cast<void (*)()>(dlsym(dlopenRos, "_Z9RunTurtlev"));
                // auto Moveturtle = reinterpret_cast<void (*)()>(dlsym(dlopenRos, "_Z7RunTestv"));

                if (Moveturtle == NULL)
                {
                        std::cout << "dlsym failed" << dlerror() << std::endl;
                        handle_pub.init();
                        handle_pub.log_pub_message(dlerror());
                        return;
                }

                Moveturtle(); // roop
                dlclose(dlopenRos);
                return;
                // rcvdecision_thrd.join();
        }
}

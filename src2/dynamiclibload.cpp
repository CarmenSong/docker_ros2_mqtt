#include <iostream>
#include <filesystem>
#include <chrono>
#include <filesystem>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <thread>
#include <memory>

#include "dlfcn.h"
#include "unistd.h"
#include "libupdate.cpp"
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using mesg::Lib_Container;

void loading_turtlesim_lib()
{
        std::cout << "Using Lib: " << Lib_Container[Lib_Container.size() - 1] << std::endl;
        char *lib_path = const_cast<char *>(Lib_Container[Lib_Container.size() - 1].c_str());
        // std::cout << Lib_container[0] << std::endl;
        const auto dlopenRos = dlopen(lib_path, RTLD_NOW);
        auto Moveturtle = reinterpret_cast<void (*)(int, int)>(dlsym(dlopenRos, "_Z9RunTurtleii"));
        Moveturtle(1, 1);
        dlclose(dlopenRos);
}

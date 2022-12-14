#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <thread>

#include "grpcpp/grpcpp.h"
#include "Message.pb.h"
#include "Message.grpc.pb.h"
#include "mosquitto.h"
#include "Turtlesim_auto.cpp"
#include "Turtlesim_manual.h"
#include "MessageHandlerSub.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
// not using namespace mesg

std::string received_msg;
std::string received_keyboard;

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    mesg::MessageHandler handle; // grpc channel

    // mesg::stateofturtle_pub turtlestate;
    // turtlestate.stateofturtle_pub_init();
    // turtlestate.stateofturtle_pubmsg("STOP");

    handle.init();
    handle.call_back();

    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();
    rclcpp::shutdown();

    return 0;
}
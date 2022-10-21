#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <thread>

#include "grpcpp/grpcpp.h"
#include "Message.pb.h"
#include "spdlog/spdlog.h"
#include "Message.grpc.pb.h"
#include "mosquitto.h"
#include "Turtlesim_auto.cpp"
#include "Turtlesim_manual.h"
#include "MessageHandlerSub.h"
// #include "MessageHandlerPub.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
// not using namespace mesg

std::string received_msg;
std::string received_keyboard;

class mesg::MessageHandler;
// class mesg::MessageHandlerPub;

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    spdlog::info("Hello Everybody!!!");
    mesg::MessageHandler handle(
        grpc::CreateChannel("localhost:9515", grpc::InsecureChannelCredentials())); // grpc channel

    handle.init();
    handle.call_back();
    // std::thread thrd1(handle.call_back);
    // thrd1.join();

    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();
    rclcpp::shutdown();

    return 0;
}
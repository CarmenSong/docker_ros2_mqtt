#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <thread>
#include <stdlib.h>

#include "grpcpp/grpcpp.h"
#include "Message.pb.h"
#include "Message.grpc.pb.h"
#include "mosquitto.h"
#include "MessageHandlerPub.h"
#include "read_key.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_sinks.h"
// #include "RPCHandlerPub.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

// using mesg::Reply;

int main(int argc, char *argv[])
{

    static std::string key;
    mesg::MessageHandlerPub handle; // grpc channel
    // auto console = spdlog::stdout_logger_mt("console");
    std::thread logthrd([&]()
                        { handle.log_init(); });

    mosquitto_lib_init();
    // handle.init();
    spdlog::info("STATE: POWER OFF");

    while (true)
    {
        handle.init();
        std::cout << "1-automatic, 2-manual, 3-exit, 4-reload, 5-precheck, 6-RPC_command" << std::endl;

        std::cin >> key;
        handle.pub_message(key);

        while (key == "2")
        {
            std::cout << "A-Forward, B-Back, C-Right, D-Left, E-Exit" << std::endl;
            // spdlog::info("A-Forward, B-Back, C-Right, D-Left, E-Exit");
            handle.init();
            std::string keyboard = read_keyboard();
            // std::cout << keyboard << std::endl;
            handle.pub_message(keyboard);
            if (keyboard == "69")
                break;
        }

        // if (key == "6")
        // {
        //     rpc_commond();
        // }
    }
    logthrd.join();
    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
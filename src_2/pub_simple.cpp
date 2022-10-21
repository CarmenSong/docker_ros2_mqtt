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
#include "MessageHandlerPub.h"
#include "read_key.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

// using mesg::Reply;

class mesg::MessageHandlerPub;

int main(int argc, char *argv[])
{
    static std::string key;
    mesg::MessageHandlerPub handle_pub; // grpc channel

    mosquitto_lib_init();
    // handle.init();

    while (true)
    {
        handle_pub.init();

        std::cout << "1-automatic, 2-manual, 3-exit, 4-reload_library" << std::endl;
        std::cin >> key;
        handle_pub.pub_message(key);
        // if (key == "2")
        // {
        // keyboard_init();
        // }
        while (key == "2")
        {
            // std::cout << "hello world" << std::endl;
            handle_pub.init();
            std::string keyboard = read_keyboard();
            // std::cout << keyboard << std::endl;
            // std::cout << "hello" << std::endl;
            handle_pub.pub_message(keyboard);
            if (keyboard == "69")
                break;
        }
    }
    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}

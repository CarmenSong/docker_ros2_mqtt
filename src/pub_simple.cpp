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
    mesg::MessageHandlerPub handle(
        grpc::CreateChannel("localhost:9515", grpc::InsecureChannelCredentials())); // grpc channel

    mosquitto_lib_init();
    // handle.init();

    while (true)
    {
        handle.init();

        std::cout << "1-automatic, 2-manual, 3-exit" << std::endl;
        std::cin >> key;
        handle.pub_message(key);
        // if (key == "2")
        // {
        // keyboard_init();
        // }
        while (key == "2")
        {
            // std::cout << "hello world" << std::endl;
            handle.init();
            std::string keyboard = read_keyboard();
            // std::cout << keyboard << std::endl;
            // std::cout << "hello" << std::endl;
            handle.pub_message(keyboard);
            if (keyboard == "69")
                break;
        }
    }
    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}

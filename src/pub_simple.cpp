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

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

// using mesg::Reply;

class mesg::MessageHandlerPub;

int main(int argc, char *argv[])
{
    static std::string key;
    mesg::MessageHandlerPub handle(
        grpc::CreateChannel("localhost:1883", grpc::InsecureChannelCredentials()));

    mosquitto_lib_init();

    while (true)
    {
        handle.init();
        std::cin >> key;
        handle.pub_message(key);
    }

    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}

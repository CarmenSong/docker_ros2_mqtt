#include "LogHandlerSub.h"


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

namespace mesg
{
    static Message message_pub;

    class MessageHandlerPub
    {
    public:
        MessageHandlerPub() {}

        void init() // mqtt initialization
        {
            mosq = mosquitto_new("publisher-test", true, NULL);

            mesg::LogHandlerSub log_handle_sub; // grpc channel

            log_handle_sub.init(); //initialization of LogHandlerSub
            // log_handle.log_call_back();

            rc = mosquitto_connect(mosq, "localhost", 1883, 60);
            if (rc != 0)
            {
                std::cout << "Cannot connect to broker " << rc << std::endl;
                mosquitto_destroy(mosq);
                return;
            }
            // std::cout << "Broker Connection Success" << std::endl;
        }

        int pub_message(const std::string &m)
        {
            Message message_pub;
            message_pub.set_body(m);

            std::string message_string;
            message_pub.SerializeToString(&message_string); // Serializes the message and stroes in a given string
            // here is message_string
            // std::cout <<  message_string << std::endl;
            mosquitto_publish(mosq, nullptr, mesg_pub_topic.c_str(),
                              static_cast<int>(message_string.length() + 1),
                              message_string.c_str(), 0, false);

            mosquitto_disconnect(mosq);
            mosquitto_destroy(mosq);

            mosquitto_lib_cleanup();

            return 0;
        }

    private:
        struct mosquitto *mosq;
        std::unique_ptr<Transfer::Stub> stub_;
        int rc;
        std::string mesg_pub_topic = "MessageHandler/publish";
    };
}
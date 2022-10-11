using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

namespace mesg
{
    static Message messagekey;

    class KeyboardHandler_Pub
    {
    public:
        KeyboardHandler_Pub(std::shared_ptr<Channel> channel_key)
            : stub_key(Transfer::NewStub(channel_key)) {}

        void init() // mqtt initialization
        {
            mosq = mosquitto_new("keyboard", true, NULL);

            rc = mosquitto_connect(mosq, "localhost", 1883, 60);
            if (rc != 0)
            {
                std::cout << "Cannot connect to broker " << rc << std::endl;
                mosquitto_destroy(mosq);
                return;
            }
            std::cout << "Broker Connection Success" << std::endl;
        }

        int pub_message(const std::string &m)
        {
        //     Message message;
            messagekey.set_body(m);

            std::string message_string;
            messagekey.SerializeToString(&message_string); // Serializes the message and stroes in a given string
            // here is message_string
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
        std::unique_ptr<Transfer::Stub> stub_key;
        int rc;
        std::string mesg_pub_topic = "MessageHandler/publish_key";
    };
}

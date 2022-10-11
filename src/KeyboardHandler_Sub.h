using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using mesg::Message;
// using mesg::Reply;
using mesg::Transfer;

std::string received_keyboard;
static Message messagekey;

// mosquitto broker connection
void on_connect_key(struct mosquitto *mosq, void *obj, int rc)
{
    if (rc)
    {
        std::cout << "Error" << std::endl;
        exit(-1);
    }
    mosquitto_subscribe(mosq, NULL, "MessageHandler/publish_key", 0);
}
// mosquitto data transformation
void on_message_key(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    std::cout << "From Topic " << msg->topic << std::endl;
    message.ParseFromArray((const char *)msg->payload, msg->payloadlen);
    // std::cout << "Got Message: " << message.DebugString();
    std::string result = message.body();
    std::cout << result << std::endl;
}

class KeyboardHandler_Sub
{
public:
    KeyboardHandler_Sub(std::shared_ptr<Channel> channel_key)
        : stub_key(Transfer::NewStub(channel_key)) {}

    void init() // mqtt initialization
    {
        // init publish struct
        mosq = mosquitto_new("keyboard", true, &id);
        mosquitto_connect_callback_set(mosq, on_connect_key);
        mosquitto_message_callback_set(mosq, on_message_key);

        auto register_callback_key = [=] { // subscribe data from broker
            mosquitto_subscribe_callback(
                &KeyboardHandler_Sub::call_back_func, NULL,
                "MessageHandler/publish_key", 0,
                "localhost", 1883,
                NULL, 10, true,
                NULL, NULL,
                NULL, NULL);
        };

        std::thread register_callback_thread_key(register_callback_key);
        register_callback_thread_key.detach();
        // std::cout << "Finish" << std::endl;

        rc = mosquitto_connect(mosq, "localhost", 1883, 60);
        if (rc)
        {
            std::cout << "Connection Fail" << std::endl;
            return;
        }
    }

    // serialize subscribed protoc data
    static int call_back_func(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
    {
        std::cout << "From Topic: " << msg->topic << std::endl;
        messagekey.ParseFromArray((const char *)msg->payload, msg->payloadlen);
        std::cout << "keyboard: " << messagekey.body() << std::endl;
        received_keyboard = message.body();

        return 0;
    }
    // exit the connection to broker
    void call_back()
    {
        mosquitto_loop_start(mosq);
        // while (true)
        // {
        //     // std::cout << "press and key to exit" << std::endl;
        //     auto key = received_keyboard;

        // }
        mosquitto_loop_stop(mosq, true);
        mosquitto_disconnect(mosq);
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
    }

private:
    struct mosquitto *mosq;
    std::unique_ptr<Transfer::Stub> stub_key;
    std::string _host;
    int id = 12;
    int rc;
    std::string mesg_pub_topic = "MessageHandler/publish_key";
};

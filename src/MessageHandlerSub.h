using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using mesg::Message;
// using mesg::Reply;
using mesg::Transfer;

std::string received_msg;
static Message message;

// mosquitto broker connection
void on_connect(struct mosquitto *mosq, void *obj, int rc)
{
    if (rc)
    {
        std::cout << "Error" << std::endl;
        exit(-1);
    }
    mosquitto_subscribe(mosq, NULL, "MessageHandler/publish", 0);
}
// mosquitto data transformation
void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    std::cout << "From Topic " << msg->topic << std::endl;
    message.ParseFromArray((const char *)msg->payload, msg->payloadlen);
    // std::cout << "Got Message: " << message.DebugString();
    std::string result = message.body();
    std::cout << result << std::endl;
}

class MessageHandler_Sub
{
public:
    MessageHandler_Sub(std::shared_ptr<Channel> channel)
        : stub_(Transfer::NewStub(channel)) {}

    void init() // mqtt initialization
    {
        // init publish struct
        mosq = mosquitto_new("subsrcibe-test", true, &id);
        mosquitto_connect_callback_set(mosq, on_connect);
        mosquitto_message_callback_set(mosq, on_message);

        auto register_callback = [=] { // subscribe data from broker
            mosquitto_subscribe_callback(
                &MessageHandler_Sub::call_back_func, NULL,
                "MessageHandler/publish", 0,
                "localhost", 1883,
                NULL, 10, true,
                NULL, NULL,
                NULL, NULL);
        };

        std::thread register_callback_thread(register_callback);
        register_callback_thread.detach();
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
        message.ParseFromArray((const char *)msg->payload, msg->payloadlen);
        std::cout << "message: " << message.body() << std::endl;
        received_msg = message.body();

        return 0;
    }
    // exit the connection to broker
    void call_back()
    {
        mosquitto_loop_start(mosq);
        while (true)
        {
            // std::cout << "press and key to exit" << std::endl;
            auto c = received_msg;
            if (c == "3")
            {
                break;
            }
            else if (c == "1")
            {
                RunTurtle();
            }
            else if (c == "2")
            {
                turtlesim_manual();
            }
        }
        mosquitto_loop_stop(mosq, true);
        mosquitto_disconnect(mosq);
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
    }

private:
    struct mosquitto *mosq;
    std::unique_ptr<Transfer::Stub> stub_;
    std::string _host;
    int id = 12;
    int rc;
    std::string mesg_pub_topic = "MessageHandler/publish";
};

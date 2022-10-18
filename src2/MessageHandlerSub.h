#include "dynamiclibload.cpp"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

extern std::string received_msg;
extern std::string received_keyboard;

namespace mesg
{
    // mosquitto broker connection
    static Message message;

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
        // std::cout << "From Topic" << msg->topic << std::endl;
        message.ParseFromArray((const char *)msg->payload, msg->payloadlen);
        // std::cout << "Got Message: " << message.DebugString();
        //     std::string result = message.body();
        //     std::cout << result << std::endl;
    }

    // grpc+mqtt
    class MessageHandler
    {
    public:
        MessageHandler(std::shared_ptr<Channel> channel)
            : stub_(Transfer::NewStub(channel)) {}

        void init() // mqtt initialization
        {
            // init publish struct
            mosq = mosquitto_new("subsrcibe-test", true, &id);
            mosquitto_connect_callback_set(mosq, on_connect);
            mosquitto_message_callback_set(mosq, on_message);

            auto register_callback = [=] { // subscribe data from broker
                mosquitto_subscribe_callback(
                    &MessageHandler::call_back_func, NULL,
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
            received_keyboard = message.body();

            return 0;
        }
        // exit the connection to broker
        void call_back()
        {
            mosquitto_loop_start(mosq);
            while (received_msg != "3")
            {
                if (received_msg == "1")
                {
                    // loading_turtlesim_lib();
                    RunTurtle(1,1);
                }
                if (received_msg == "2")
                {
                    while (received_keyboard != "69") //if key "E" exit
                    {
                        // std::cout << "hello" << std::endl;
                        turtlesim_manual();
                    }
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
}
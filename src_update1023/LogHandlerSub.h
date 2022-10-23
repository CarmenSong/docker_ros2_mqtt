// #include "dynamiclibload.cpp"
#include "spdlog/spdlog.h"

namespace mesg
{
    // mosquitto broker connection
    static Message log;

    void log_on_connect(struct mosquitto *mosq, void *obj, int rc)
    {
        if (rc)
        {
            std::cout << "Error" << std::endl;
            exit(-1);
        }
        mosquitto_subscribe(mosq, NULL, "MessageHandler/publish_log", 0);
    }
    // mosquitto data transformation
    void log_on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
    {
        // std::cout << "From Topic" << msg->topic << std::endl;
        log.ParseFromArray((const char *)msg->payload, msg->payloadlen);
        // std::cout << "Got Message: " << message.DebugString();
        //     std::string result = message.body();
        //     std::cout << result << std::endl;
    }

    // grpc+mqtt
    class LogHandlerSub
    {
    public:
        LogHandlerSub() {}

        void init() // mqtt initialization
        {
            // init publish struct
            mosq_log = mosquitto_new("subsrcibe-log", true, &id);
            mosquitto_connect_callback_set(mosq_log, log_on_connect);
            mosquitto_message_callback_set(mosq_log, log_on_message);

            auto log_register_callback = [=] { // subscribe data from broker
                mosquitto_subscribe_callback(
                    &LogHandlerSub::log_call_back_func, NULL,
                    "MessageHandler/publish_log", 0,
                    "localhost", 1883,
                    NULL, 10, true,
                    NULL, NULL,
                    NULL, NULL);
            };

            std::thread log_register_callback_thread(log_register_callback);
            log_register_callback_thread.detach();
            
            rc = mosquitto_connect(mosq_log, "localhost", 1883, 60);
            if (rc)
            {
                std::cout << "Connection Fail" << std::endl;
                return;
            }
        }

        // serialize subscribed protoc data
        static int log_call_back_func(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
        {
            // std::cout << "From Topic: " << msg->topic << std::endl;
            log.ParseFromArray((const char *)msg->payload, msg->payloadlen);
            spdlog::info(log.body());
            // std::cout << log.body() << std::endl;
            sleep(1);
            // std::cout << "message: " << log.body() << std::endl;

            return 0;
        }



    private:
        struct mosquitto *mosq_log;

        int id = 14;
        int rc;
        std::string mesg_pub_topic = "MessageHandler/publish_log";
    };
}
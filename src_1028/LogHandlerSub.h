// #include "dynamiclibload.cpp"
#include "spdlog/spdlog.h"
// #include "spdlog/fmt/ostr.h"
// #include "spdlog/sinks/stdout_sinks.h"

namespace mesg
{
    // mosquitto broker connection
    static Message log;

    // extern auto console = spdlog::stdout_logger_mt("console");

    int state_of_turtle(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
    {
        // std::cout << "From Topic" << msg->topic << std::endl;
        spdlog::info((char *)msg->payload);
        return 0;
    }

    int roslog_on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
    {
        // std::cout << "New message: " << msg->topic << (char *)msg->payload << std::endl;
        spdlog::info((char *)msg->payload);
        return 0;
    }

    // grpc+mqtt
    class LogHandlerSub
    {
    public:
        LogHandlerSub() {}

        void stateofturtle_init()
        {
            mosq_stateofturtle = mosquitto_new("subscribe-test", true, &id);

            rc = mosquitto_connect(mosq_stateofturtle, "localhost", 1883, 10);
            if (rc)
            {
                std::cout << "Connection Fail" << std::endl;
                return;
            }
            auto stateofturtle_callback = [=] { // subscribe data from broker
                mosquitto_subscribe_callback(
                    &state_of_turtle, NULL, "roslog2", 0,
                    "localhost", 1883, NULL, 10, true,
                    NULL, NULL, NULL, NULL);
            };

            std::thread rosnodelog_callback_thread(stateofturtle_callback);
            rosnodelog_callback_thread.detach();
        }

        void rosnodelog_init()
        {
            mosq_roslog = mosquitto_new("subscribe-test", true, &id);

            rc = mosquitto_connect(mosq_roslog, "localhost", 1883, 10);
            if (rc)
            {
                std::cout << "Connection Fail" << std::endl;
                return;
            }
            auto rosnodelog_register_callback = [=] { // subscribe data from broker
                mosquitto_subscribe_callback(
                    &roslog_on_message, NULL, "roslog", 0,
                    "localhost", 1883, NULL, 10, true,
                    NULL, NULL, NULL, NULL);
            };

            std::thread rosnodelog_callback_thread(rosnodelog_register_callback);
            rosnodelog_callback_thread.detach();
        }

        void init() // mqtt initialization
        {
            // init publish struct
            mosq_log = mosquitto_new("subsrcibe-log", true, &id);

            auto log_register_callback = [=] { // subscribe data from broker
                mosquitto_subscribe_callback(
                    &LogHandlerSub::log_call_back_func, NULL,
                    "MessageHandler/publish_log", 0,
                    "localhost", 1883, NULL, 10, true,
                    NULL, NULL, NULL, NULL);
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
            // std::cout << "message: " << log.body() << std::endl;

            return 0;
        }

    private:
        struct mosquitto *mosq_log;
        struct mosquitto *mosq_roslog;
        struct mosquitto *mosq_stateofturtle;
        int id = 14;
        int rc;
        std::string mesg_pub_topic = "MessageHandler/publish_log";
    };
}
// #include "MessageHandlerSub.h"

namespace mesg
{
    static Message log_pub;

    class LogHandlerPub
    {
    public:
        LogHandlerPub() {}

        void init() // mqtt initialization
        {
            mosq = mosquitto_new("subsrcibe-test", true, NULL);

            rc = mosquitto_connect(mosq, "localhost", 1883, 60);
            if (rc != 0)
            {
                std::cout << "Cannot connect to broker " << rc << std::endl;
                mosquitto_destroy(mosq);
                return;
            }
            // std::cout << "Broker Connection Success" << std::endl;
        }

        int log_pub_message(const std::string &m)
        {
            Message log_pub;
            log_pub.set_body(m);

            std::string log_string;
            log_pub.SerializeToString(&log_string); // Serializes the message and stroes in a given string
            // here is message_string
            // std::cout <<  message_string << std::endl;
            mosquitto_publish(mosq, nullptr, mesg_pub_topic.c_str(),
                              static_cast<int>(log_string.length() + 1),
                              log_string.c_str(), 0, false);

            mosquitto_disconnect(mosq);
            mosquitto_destroy(mosq);

            mosquitto_lib_cleanup();

            return 0;
        }

    private:
        struct mosquitto *mosq;
        int rc;
        std::string mesg_pub_topic = "MessageHandler/publish_log";
    };

    class stateofturtle_pub
    {
    public:
        stateofturtle_pub()
        {
            mosquitto_lib_init();
        }

        void stateofturtle_pub_init()
        {
            mosq_stateofturtle = mosquitto_new("subsrcibe-test", true, NULL);
            rc = mosquitto_connect(mosq_stateofturtle, "localhost", 1883, 60);
            if (rc != 0)
            {
                std::cout << "Cannot connect to broker " << rc << std::endl;
                mosquitto_destroy(mosq_stateofturtle);
                return;
            }
        }

        void stateofturtle_pubmsg(const std::string &m)
        {
            mosquitto_publish(mosq_stateofturtle, nullptr, "roslog2",
                              sizeof(m),
                              m.c_str(), 0, false);

            mosquitto_disconnect(mosq_stateofturtle);
            mosquitto_destroy(mosq_stateofturtle);
            mosquitto_lib_cleanup();
            return;
        }

    private:
        struct mosquitto *mosq;
        struct mosquitto *mosq_stateofturtle;
        int rc;
    };
}
#include "dynamiclibload.cpp"
// #include "LogHandlerPub.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

extern std::string received_msg;

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
        
        // mesg::LogHandlerPub handle_pub;

        void init() // mqtt initialization
        {
            // init publish struct
            mosq = mosquitto_new("subsrcibe-test", true, &id);
            mosquitto_connect_callback_set(mosq, on_connect);
            mosquitto_message_callback_set(mosq, on_message);

            // loghandler mqtt pub
            

            // handle_pub.init();
            //

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
            std::thread thrd2(check_lib_update, initial_dirctime, dirc_time);
            // std::thread thrd3(log_libupdate);
            while (received_msg != "3")
            {
                
                // iflibupdate = check_lib_update(initial_dirctime, dirc_time);
                
                // if (iflibupdate) {
                //     initial_dirctime = get_dirctime(dirc_time);
                //     handle_pub.init();
                //     handle_pub.log_pub_message("dynamic-lib update success");
                //     iflibupdate = false;
                // }

                if (received_msg == "1")
                {
                    // handle_pub.log_pub_message("")
                    std::thread thrd1(mesg::loading_turtlesim_lib);
                    thrd1.join();
                }

                if (received_msg == "2")
                {
                    while (received_keyboard != "69") // if key "E" exit
                    {
                        std::thread thrd1(turtlesim_manual);
                        thrd1.join();
                    }
                }

                if (received_msg == "4")
                {
                    received_msg = "1";
                }
            }
            thrd2.join();
            // thrd3.join();
            mosquitto_loop_stop(mosq, true);
            mosquitto_disconnect(mosq);
            mosquitto_destroy(mosq);
            mosquitto_lib_cleanup();
        }

        // protected:
        //     std::string received_msg;
    private:
        struct mosquitto *mosq;
        std::unique_ptr<Transfer::Stub> stub_;

        std::string _host;
        int id = 12;
        int rc;
        std::string mesg_pub_topic = "MessageHandler/publish";
    };
}
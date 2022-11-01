#include <stdlib.h>
// #include "dynamiclibload.cpp"

extern std::string received_msg;

class RPCHandlerSub
{
public:
        void init() // mqtt initialization
        {
                // init publish struct
                mosq = mosquitto_new("subsrcibe-test", true, &id);

                auto register_callback = [=] { // subscribe data from broker
                        mosquitto_subscribe_callback(
                            &RPCHandlerSub::call_back_func, NULL,
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

                // command_rcv = (char *)msg->payload;
                while (received_msg != "3" )
                {
                        if (received_msg == "6")
                        {
                                int result = system("wget https://raw.githubusercontent.com/CarmenSong/TestGrpc/main/ros2_update_lib/libdlopenRos_update.so");
                                received_msg = "1";
                                // break;
                        } 
                        if (received_msg == "5")
                        {
                                mesg::updatedlib_precheck();
                                received_msg = "1";
                        }
                        if (received_msg != "1")
                        {
                                break;
                        }

                }
                return 0;
        }
        // exit the connection to broker

        // protected:
        //     std::string received_msg;
private:
        struct mosquitto *mosq;
        std::string _host;
        int id = 12;
        int rc;
};
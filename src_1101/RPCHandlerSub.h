#include <stdlib.h>

char* command_rcv;

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
                            "test/t1", 0,
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

                command_rcv = (char *)msg->payload;
                int result = system(command_rcv);
                // std::cout << command_rcv << std::endl;
                return 0;
        }
        // exit the connection to broker
        void call_back()
        {
                mosquitto_loop_start(mosq);
                getchar();
                mosquitto_loop_stop(mosq, true);
                mosquitto_disconnect(mosq);
                mosquitto_destroy(mosq);
                mosquitto_lib_cleanup();
        }

        // protected:
        //     std::string received_msg;
private:
        struct mosquitto *mosq;
        std::string _host;
        int id = 12;
        int rc;
};

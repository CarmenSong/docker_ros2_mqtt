#include "rclcpp/rclcpp.hpp"
#include <geometry_msgs/msg/twist.hpp>
#include "mosquitto.h"
// #include "Message.pb.h"
// #include "LogHandlerPub.h"

extern std::string received_msg;

namespace rpc
{
        class roslog_pub
        {
        public:
                roslog_pub()
                {
                        mosquitto_lib_init();
                }

                void roslog_pub_init()
                {
                        mosq = mosquitto_new("subsrcibe-test", true, NULL);
                        rc = mosquitto_connect(mosq, "localhost", 1883, 60);
                        if (rc != 0)
                        {
                                std::cout << "Cannot connect to broker " << rc << std::endl;
                                mosquitto_destroy(mosq);
                                return;
                        }
                }

                void roslog_pubmsg(const std::string &m)
                {
                        mosquitto_publish(mosq, nullptr, "roslog",
                                          sizeof(m),
                                          m.c_str(), 0, false);

                        mosquitto_disconnect(mosq);
                        mosquitto_destroy(mosq);
                        mosquitto_lib_cleanup();
                        return;
                }

        private:
                struct mosquitto *mosq;
                int rc;
        };
} // namespace name

// mesg::LogHandlerPub roshandle_pub;

void RunTurtle()
{

        auto node = rclcpp::Node::make_shared("turtle_controller");
        auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 1);
        int count = 0;
        rpc::roslog_pub rosnode;
        rosnode.roslog_pub_init();
        rosnode.roslog_pubmsg("Node msg publish success!"); 
        // lib_management();
        // int Initial_Lib_Number = Lib_Container.size();
        // std::cout << "Initial Lib Number: " << Initial_Lib_Number << std::endl;

        while (received_msg == "1" || received_msg == "5")
        {
                auto msg = geometry_msgs::msg::Twist();
                msg.linear.x = 4;
                msg.angular.z = 2;
                publisher->publish(msg);
        }

        return;
}
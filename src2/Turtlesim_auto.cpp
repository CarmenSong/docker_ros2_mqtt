#include "rclcpp/rclcpp.hpp"
#include <geometry_msgs/msg/twist.hpp>

extern std::string received_msg;

void RunTurtle(int a, int b) {
        
        auto node = rclcpp::Node::make_shared("turtle_controller");
        auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",1);
        int count = 0;
        //lib_management();
        //int Initial_Lib_Number = Lib_Container.size();
        //std::cout << "Initial Lib Number: " << Initial_Lib_Number << std::endl;

        while (received_msg == "1") {
                auto msg = geometry_msgs::msg::Twist();
                msg.linear.x = a;
                msg.angular.z = b;
                publisher->publish(msg);
        }

        return;
}
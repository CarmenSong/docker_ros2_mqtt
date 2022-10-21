#include "rclcpp/rclcpp.hpp"
#include <geometry_msgs/msg/twist.hpp>
#include "std_msgs/msg/string.hpp"
// #include "dynamiclibload.cpp"

#include <functional>
#include <memory>
#include <string>
// 
extern std::string received_msg;
// extern bool mesg::iflibupdate;

void RunTurtle()
{

        auto node = rclcpp::Node::make_shared("turtle_controller");
        auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 1);

        while (received_msg == "1")
        {
                auto msg = geometry_msgs::msg::Twist();
                msg.linear.x = 1;
                msg.angular.z = 1;
                publisher->publish(msg);

        }

        return;
}

// class MoveTurtle : public rclcpp::Node
// {
// public:
//         MoveTurtle() : Node("turtle_controller")
//         {
//                 auto publisher = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 1);
//                 auto msg = geometry_msgs::msg::Twist();
//                 msg.linear.x = 1;
//                 msg.angular.z = 1;
//                 RCLCPP_INFO(this->get_logger(), "Connection success");
//                 publisher->publish(msg);
//         }
// };

// void RunTest ()
// {
//         std::make_shared<MoveTurtle>();
// }


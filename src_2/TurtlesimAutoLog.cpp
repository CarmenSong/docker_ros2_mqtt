
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/string.hpp"

#include <chrono>
#include <functional>
#include <memory>
#include <string>

class MoveTurtle : public rclcpp::Node
{
public:
        MoveTurtle() : Node("turtle_controller")
        {
                // auto node = rclcpp::Node::make_shared("turtle_controller");
                auto publisher = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 1);

                auto msg = geometry_msgs::msg::Twist();
                msg.linear.x = 2.0;
                msg.angular.z = 2.0;
                int count = 0;
                RCLCPP_INFO(this->get_logger(), "Ready");
                rclcpp::WallRate loop(1);
                while (rclcpp::ok())
                {
                        publisher->publish(msg);
                        RCLCPP_INFO(this->get_logger(), "Publish success %d", count);
                        count++;
                        loop.sleep();
                        // RCLCPP_DEBUG(this->get_logger(), "Sth wrong! Please Retry! ");
                        RCLCPP_ERROR(this->get_logger(), "Something wroing, Please Check");
                }
        }
};

int main(int argc, char **argv)
{
        rclcpp::init(argc, argv);
        std::make_shared<MoveTurtle>();
        rclcpp::shutdown();
        return 0;
}
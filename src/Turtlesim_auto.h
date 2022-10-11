extern std::string received_msg;

void RunTurtle() {
        
        auto node = rclcpp::Node::make_shared("turtle_controller");
        auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",1);

        rclcpp::WallRate loop(5);
        while (received_msg == "1") {
                auto msg = geometry_msgs::msg::Twist();
                msg.linear.x = 1.0;
                msg.angular.z = 1.0;
                publisher->publish(msg);
                loop.sleep();
        }

}

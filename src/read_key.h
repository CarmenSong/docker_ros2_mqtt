#include <signal.h>
#include <termios.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <string>

int kfd = 0;
struct termios cooked, raw;
extern std::string received_msg;

#define KEYCODE_R 0x43
#define KEYCODE_L 0x44
#define KEYCODE_U 0x41
#define KEYCODE_D 0x42
#define KEYCODE_Q 0x71
#define KEYCODE_C 0x45

void quit(int sig)
{
        (void)sig;
        tcsetattr(kfd, TCSANOW, &cooked);
        //   ros::shutdown();
        // rclcpp::shutdown();
        exit(0);
}

void keyboard_init(void)
{
        signal(SIGINT, quit);

        // get the console in raw mode
        tcgetattr(kfd, &cooked);
        memcpy(&raw, &cooked, sizeof(struct termios));
        raw.c_lflag &= ~(ICANON | ECHO);
        // Setting a new line, then end of file
        raw.c_cc[VEOL] = 1;
        raw.c_cc[VEOF] = 2;
        tcsetattr(kfd, TCSANOW, &raw);
}

std::string read_keyboard()
{
        char c;
        static std::string key;
        // get the next event from the keyboard
        if (::read(kfd, &c, 1) < 0)
        {
                perror("read():");
                exit(-1);
        }

        switch (c)
        {
        case KEYCODE_L:
                std::cout << "TURN LEFT" << std::endl;
                // std::cout << c << std::endl;
                key = std::to_string(c);
                break;
        case KEYCODE_R:
                std::cout << "TURN RIGHT" << std::endl;
                key = std::to_string(c);
                break;
        case KEYCODE_U:
                std::cout << "Forward" << std::endl;
                key = std::to_string(c);
                break;
        case KEYCODE_D:
                std::cout << "BACK" << std::endl;
                key = std::to_string(c);
                break;
        case KEYCODE_C:
                std::cout << "EXIT" << std::endl;
                key = std::to_string(c);
                break;
        default:
                break;
        }

        // std::string key = std::to_string(c);
        return key;
}

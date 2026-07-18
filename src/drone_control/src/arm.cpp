#include "drone_control.h"
using namespace std::chrono_literals;

class ArmNode : public DroneControl
{
public:
    ArmNode() : DroneControl ()
    {

        auto timer_callback = [this]()->void {
            if (offboard_setpoint_counter_ == 10) {
                this->arm();
                RCLCPP_INFO(this->get_logger(), "Arming...");
            }

            if (offboard_setpoint_counter_==110) {
                this->disarm();
                RCLCPP_INFO(this->get_logger(), "Disarming after 5 seconds...");
            }

            if (offboard_setpoint_counter_<100) {
                offboard_setpoint_counter_++;
            }
        };
        timer_ = this->create_wall_timer(100ms, timer_callback);
    }

};

// === Main function ===
int main(int argc, char *argv[])
{
    std::cout << "Starting drone takeoff node..." << std::endl;
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ArmNode>());
    rclcpp::shutdown();
    return 0;
}

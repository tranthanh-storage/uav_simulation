#include "drone_control.h"

using namespace std::chrono_literals;

class TakeoffNode : public DroneControl
{
public:
    TakeoffNode() : DroneControl()
    {
        auto timer_callback = [this]() -> void {

            if (offboard_setpoint_counter_ == 10) {
                // Change to offboard mode after 10 setpoints
                this->offboard_mode();

                // Arm the vehicle
                this->arm();
            }

            if (offboard_setpoint_counter_ == 150) {
                // Land after 150 setpoints
                this->land();
                this->disarm();
            }

            // Stop the counter after 150 setpoints
            if (offboard_setpoint_counter_ < 151) {
                offboard_setpoint_counter_++;
            }

            // offboard_control_mode needs to be paired with trajectory_setpoint
            publish_offboard_control_mode();
            publish_trajectory_setpoint(0.0, 0.0, -3);
        };
        // Create a timer that calls the callback every 100 milliseconds
        timer_ = this->create_wall_timer(100ms, timer_callback);
    }
};

// === Main function ===
int main(int argc, char *argv[])
{
    std::cout << "Starting drone takeoff node..." << std::endl;
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TakeoffNode>());
    rclcpp::shutdown();
    return 0;
}

#include "drone_control.h"
#include <cmath> // Required for NAN

using namespace std::chrono_literals;

class FlightNode : public DroneControl
{
public:
    FlightNode() : DroneControl()
    {
        auto timer_callback = [this]() -> void {

            // 1. Determine our target position based on the timeline
            float target_x = 0.0;
            float target_y = 0.0;
            float target_z = -1.5; // Z is down in NED, so -2.5 means 2.5m up

            if (offboard_setpoint_counter_ < 80) {
                // 0 to 10 seconds: Take off and hover at origin
                target_x = 0.0;
            } else if (offboard_setpoint_counter_ < 160) {
                // 10 to 20 seconds: Move 2 meters forward (North)
                target_x = 2.0;
            } else {
                // 20+ seconds: Hold the 2m position while landing
                target_x = 2.0;
            }

            // 2. CRITICAL: ALWAYS publish the mode and setpoint every tick
            publish_offboard_control_mode();
            publish_trajectory_setpoint(target_x, target_y, target_z);

            // 3. Send one-time commands at specific counter moments
            if (offboard_setpoint_counter_ == 10) {
                // We have now streamed 10 setpoints, Offboard mode will be accepted
                this->offboard_mode();
                this->arm();
            }

            if (offboard_setpoint_counter_ == 160) {
                // Initiate landing sequence
                this->land();
            }

            // 4. Increment counter (cap it to prevent overflow)
            if (offboard_setpoint_counter_ < 170) {
                offboard_setpoint_counter_++;
            }
        };

        // Create a timer that calls the callback every 100 milliseconds
        timer_ = this->create_wall_timer(100ms, timer_callback);
    }
};

int main(int argc, char *argv[])
{
    std::cout << "Starting offboard control node..." << std::endl;
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<FlightNode>());
    rclcpp::shutdown();
    return 0;
}
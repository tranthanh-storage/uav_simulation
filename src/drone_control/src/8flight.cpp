#include "drone_control.h"
#include <cmath>

using namespace std::chrono_literals;

class e_flight : public DroneControl
{
public:
    e_flight() : DroneControl()
    {
        log_file_.open("/home/tran-thanh/uav_simulation/src/uav_log.csv", std::ios::out | std::ios::trunc);
        log_file_ << "x_desired,y_desired,z_desired,x_actual,y_actual,z_actual\n";

        // Initialize publishers
        vehicle_command_pub_ = this->create_publisher<VehicleCommand>("/fmu/in/vehicle_command", 10);

        // qos profile for subscribers
        rmw_qos_profile_t qos_profile = rmw_qos_profile_sensor_data;
        auto qos = rclcpp::QoS(rclcpp::QoSInitialization(qos_profile.history, 5), qos_profile);

        // Initialize subscribers

        vehicle_local_position_sub_ = this->create_subscription<VehicleLocalPosition>(
            "/fmu/out/vehicle_local_position", qos, [this](const VehicleLocalPosition::UniquePtr msg) {
                local_position_msg_[0] = msg->y;    // (x_enu = y_ned)
                local_position_msg_[1] = msg->x;    // (y_enu = x_ned)
                local_position_msg_[2] = -(msg->z); // (z_enu = -z_ned)
            });

        auto timer_callback = [this]() -> void {

            float target_x = 0.0;
            float target_y = 0.0;
            float target_z = -2.5; // altitude (NED frame)

            // Trajectory parameters
            const float radius = 1.5;          // meters
            const float angular_speed = 0.05;  // rad per tick

            if (offboard_setpoint_counter_ < 100) {
                // Takeoff and hover
                target_x = 0.0;
                target_y = 0.0;

            } else if (offboard_setpoint_counter_ < 300) {
                // Figure-eight trajectory

                float t = (offboard_setpoint_counter_ - 100);
                float theta = angular_speed * t;

                // Smooth ∞ trajectory (Lissajous curve)
                target_x = radius * std::sin(theta);
                target_y = radius * std::sin(2 * theta) / 2.0;

            } else {
                // Hold last position
                float theta = angular_speed * (300 - 100);
                target_x = radius * std::sin(theta);
                target_y = radius * std::sin(2 * theta) / 2.0;
            }

            // Publish commands
            publish_offboard_control_mode();
            publish_trajectory_setpoint(target_x, target_y, target_z);
            log_file_ << target_x << "," << target_y << "," << target_z << ","
                          << local_position_msg_[0] << "," << local_position_msg_[1] << "," << local_position_msg_[2] << "\n";

            // Switch to offboard and arm
            if (offboard_setpoint_counter_ == 10) {
                this->offboard_mode();
                this->arm();
            }

            // Land after trajectory
            if (offboard_setpoint_counter_ == 300) {
                this->land();
            }

            // Increment counter
            if (offboard_setpoint_counter_ < 320) {
                offboard_setpoint_counter_++;
            }
        };

        timer_ = this->create_wall_timer(100ms, timer_callback);
    }
};

int main(int argc, char *argv[])
{
    std::cout << "Starting figure-eight flight node..." << std::endl;
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);

    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<e_flight>());
    rclcpp::shutdown();

    return 0;
}
#include "drone_control.h"

DroneControl::DroneControl(): Node("offboard_control")
{
    offboard_control_mode_pub_ = this->create_publisher<OffboardControlMode>("/fmu/in/offboard_control_mode", 10);
    trajectory_setpoint_pub_ = this->create_publisher<TrajectorySetpoint>("/fmu/in/trajectory_setpoint", 10);
    vehicle_command_pub_ = this->create_publisher<VehicleCommand>("/fmu/in/vehicle_command", 10);

    offboard_setpoint_counter_ = 0;
}

void DroneControl::arm()
{
    publish_vehicle_command(VehicleCommand::VEHICLE_CMD_COMPONENT_ARM_DISARM, 1);
    RCLCPP_INFO(this->get_logger(), "Arm command sent");
}

void DroneControl::disarm()
{
    publish_vehicle_command(VehicleCommand::VEHICLE_CMD_COMPONENT_ARM_DISARM, 0);
    RCLCPP_INFO(this->get_logger(), "Disarm command sent");
}

void DroneControl::land()
{
    publish_vehicle_command(VehicleCommand::VEHICLE_CMD_NAV_LAND, 0, 0);
    RCLCPP_INFO(this->get_logger(), "Land command sent");
}

void DroneControl::offboard_mode() 
{
    this->publish_vehicle_command(VehicleCommand::VEHICLE_CMD_DO_SET_MODE, 1, 6);
    RCLCPP_INFO(this->get_logger(), "Offboard mode command sent");
}

void DroneControl::publish_offboard_control_mode()
{
    OffboardControlMode msg{};
    msg.position = true;
    msg.velocity = false;
    msg.acceleration = false;
    msg.attitude = false;
    msg.body_rate = false;
    msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
    offboard_control_mode_pub_->publish(msg);
}

void DroneControl::publish_trajectory_setpoint(float x, float y, float z)
{
    TrajectorySetpoint msg{};
    msg.position = {x, y, z};
    msg.velocity = {NAN, NAN, NAN};         // Tells PX4 to ignore velocity constraints
    msg.acceleration = {NAN, NAN, NAN};     // Tells PX4 to ignore acceleration constraints
    msg.jerk = {NAN, NAN, NAN};             // Tells PX4 to ignore jerk constraints
    msg.yaw = -3.14; 
    msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
    trajectory_setpoint_pub_->publish(msg);
}

void DroneControl::publish_vehicle_command(uint16_t command, float param1, float param2)
{
    VehicleCommand msg{};
    msg.param1 = param1;
    msg.param2 = param2;
    msg.command = command;
    msg.target_system = 1;
    msg.target_component = 1;
    msg.source_system = 1;
    msg.source_component = 1;
    msg.from_external = true;
    msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
    vehicle_command_pub_->publish(msg);
}
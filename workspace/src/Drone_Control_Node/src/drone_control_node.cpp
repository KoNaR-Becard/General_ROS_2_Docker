#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>
#include <px4_msgs/msg/trajectory_setpoint.hpp>
#include <px4_msgs/msg/vehicle_command.hpp>
#include <px4_msgs/msg/vehicle_local_position.hpp>
#include <px4_msgs/msg/offboard_control_mode.hpp>

#include <chrono>
#include <cmath>

using namespace std::chrono_literals;

class DroneController : public rclcpp::Node
{
public:
    DroneController() : Node("drone_controller")
    {
        this->declare_parameter("starting_height", 2.f);
        this->declare_parameter("start_timeout", 30.f);

        this->last_position_x = 0.f;
        this->last_position_y = 0.f;
        this->last_position_z = 0.f;
        this->startSequence = true;

        this->starting_height = this->get_parameter("starting_height").as_double();

        double start_timeout = this->get_parameter("start_timeout").as_double();

        this->start_timeout_tick = static_cast<uint32_t>(start_timeout / 0.1f);

        this->offboard_counter = 0;

        this->msg_sub_ = this->create_subscription<geometry_msgs::msg::TwistStamped>("/cmd_vel_stamped", 10, std::bind(&DroneController::cmdCallback, this, std::placeholders::_1));

        this->vehicle_position_sub_ = this->create_subscription<px4_msgs::msg::VehicleLocalPosition>("/fmu/out/vehicle_local_position_v1", 10, std::bind(&DroneController::positionCallback, this, std::placeholders::_1));

        this->trajectory_pub_ = this->create_publisher<px4_msgs::msg::TrajectorySetpoint>("/fmu/in/trajectory_setpoint", 1);

        this->offboard_pub_ = this->create_publisher<px4_msgs::msg::OffboardControlMode>("/fmu/in/offboard_control_mode", 1);

        this->vehicle_command_publisher_ = this->create_publisher<px4_msgs::msg::VehicleCommand>("/fmu/in/vehicle_command", 10);

        this->timer_ = this->create_wall_timer(100ms, std::bind(&DroneController::controlLoop, this));

        this->last_msg_time_ = this->now();

        RCLCPP_INFO(this->get_logger(), "Drone controller started");
    }

private:
    void publishVehicleCommand(uint16_t command, float param1 = 0.0, float param2 = 0.0)
    {
        px4_msgs::msg::VehicleCommand msg{};
        msg.param1 = param1;
        msg.param2 = param2;
        msg.command = command;
        msg.target_system = 1;
        msg.target_component = 1;
        msg.source_system = 1;
        msg.source_component = 1;
        msg.from_external = true;
        msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
        this->vehicle_command_publisher_->publish(msg);
    }

    void cmdCallback(const geometry_msgs::msg::TwistStamped::SharedPtr msg)
    {
        current_twist_ = msg->twist;
        last_msg_time_ = this->now();
    }

    void positionCallback(const px4_msgs::msg::VehicleLocalPosition::SharedPtr msg)
    {
        this->last_position_x = msg->x;
        this->last_position_y = msg->y;
        this->last_position_z = msg->z;
    }

    void arm()
    {
        this->publishVehicleCommand(px4_msgs::msg::VehicleCommand::VEHICLE_CMD_COMPONENT_ARM_DISARM, 1.0);

        RCLCPP_INFO(this->get_logger(), "Arm command send");
    }

    void disarm()
    {
        this->publishVehicleCommand(px4_msgs::msg::VehicleCommand::VEHICLE_CMD_COMPONENT_ARM_DISARM, 0.0);

        RCLCPP_INFO(this->get_logger(), "Disarm command send");
    }

    void controlLoop()
    {

        if (offboard_counter == 200)
        {
            this->publishVehicleCommand(px4_msgs::msg::VehicleCommand::VEHICLE_CMD_DO_SET_MODE, 1.0, 6.0);

            this->arm();
            double start_timeout = this->get_parameter("start_timeout").as_double();
            this->start_timeout_tick = static_cast<uint32_t>(start_timeout / 0.1f);
        }

        publishOffboardControlMode();

        if (this->startSequence)
        {
            px4_msgs::msg::TrajectorySetpoint msg{};
            msg.position = {0.0, 0.0, -this->starting_height};
            msg.velocity = {NAN, NAN, NAN};
            msg.acceleration = {NAN, NAN, NAN};
            msg.yaw = 0.0f;
            msg.yawspeed = NAN;
            msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
            this->trajectory_pub_->publish(msg);

            if (std::abs(this->last_position_z + this->starting_height) < 0.05f){
                this->startSequence = false;
                RCLCPP_INFO(this->get_logger(), "Start sequence finished.");
            }
            else if (this->start_timeout_tick == 0)
            {
                this->startSequence = false;
                RCLCPP_WARN(this->get_logger(), "Start sequence timeout occured.");
            }
            else
            {
                this->start_timeout_tick--;
            }
        }
        else
        {
            double dt = (this->now() - last_msg_time_).seconds();
            if (dt < 0.5)
            {
                publishVelocitySetpoint(current_twist_);
            }
            else
            {
                publishHover();
            }
        }

        if (offboard_counter < 201)
        {
            offboard_counter++;
        }
    }

    void publishOffboardControlMode()
    {
        px4_msgs::msg::OffboardControlMode msg{};
        msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;

        if (this->startSequence)
        {
            msg.position = true;
            msg.velocity = false;
        }
        else
        {
            msg.position = false;
            msg.velocity = true;
        }
        msg.acceleration = false;
        msg.attitude = false;
        msg.body_rate = false;

        offboard_pub_->publish(msg);
    }

    void publishVelocitySetpoint(const geometry_msgs::msg::Twist &twist)
    {
        px4_msgs::msg::TrajectorySetpoint msg{};

        msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
        msg.position = {NAN, NAN, NAN};

        msg.velocity[0] = twist.linear.x;
        msg.velocity[1] = twist.linear.y;
        msg.velocity[2] = twist.linear.z;

        msg.acceleration = {NAN, NAN, NAN};
        msg.yaw = NAN;
        msg.yawspeed = twist.angular.z;

        trajectory_pub_->publish(msg);
    }

    void publishHover()
    {
        px4_msgs::msg::TrajectorySetpoint msg{};

        msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;

        msg.position = {NAN, NAN, NAN};
        msg.velocity = {0.0f, 0.0f, 0.0f};
        msg.acceleration = {0.0f, 0.0f, 0.0f};
        msg.yaw = NAN;
        msg.yawspeed = 0.0f;

        trajectory_pub_->publish(msg);
    }

    rclcpp::Subscription<geometry_msgs::msg::TwistStamped>::SharedPtr msg_sub_;
    rclcpp::Subscription<px4_msgs::msg::VehicleLocalPosition>::SharedPtr vehicle_position_sub_;
    rclcpp::Publisher<px4_msgs::msg::TrajectorySetpoint>::SharedPtr trajectory_pub_;
    rclcpp::Publisher<px4_msgs::msg::OffboardControlMode>::SharedPtr offboard_pub_;
    rclcpp::Publisher<px4_msgs::msg::VehicleCommand>::SharedPtr vehicle_command_publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    uint32_t offboard_counter;
    bool startSequence;
    float starting_height;
    uint32_t start_timeout_tick;

    float last_position_x;
    float last_position_y;
    float last_position_z;

    geometry_msgs::msg::Twist current_twist_;
    rclcpp::Time last_msg_time_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DroneController>());
    rclcpp::shutdown();
    return 0;
}

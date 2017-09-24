#ifndef SHIP_CONTROLLER_H_INCLUDED
#define SHIP_CONTROLLER_H_INCLUDED

//headers for controller_interface
#include <controller_interface/controller.h>

//headers for fardware_interface
#include <hardware_interface/joint_command_interface.h>

//headers for pluginlib
#include <pluginlib/class_list_macros.h>

//headers for realtime_tools
#include <realtime_tools/realtime_publisher.h>
#include <realtime_tools/realtime_buffer.h>

//headers for control_toolbox
#include <control_toolbox/pid.h>

//headers for boost
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/condition.hpp>

//headers for ROS
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float32.h>

namespace ship_controller
{
  class ShipController: public controller_interface::Controller<hardware_interface::VelocityJointInterface>
  {
  public:
    ShipController();
    //controller functions
    virtual bool init(hardware_interface::VelocityJointInterface* hw,ros::NodeHandle& controller_nh);
    virtual void starting(const ros::Time& time);
    virtual void update(const ros::Time& time, const ros::Duration& period);
    virtual void stopping(const ros::Time& time);

  private:
    //callback functions
    void cmdVelCallback(const geometry_msgs::Twist& msg);
    boost::scoped_ptr<realtime_tools::RealtimePublisher<std_msgs::Float32> > left_motor_joint_cmd_publisher;
    boost::scoped_ptr<realtime_tools::RealtimePublisher<std_msgs::Float32> > right_motor_joint_cmd_publisher;
    ros::Subscriber sub_command;
    struct Commands
    {
      double lin;
      double ang;
      ros::Time stamp;

      Commands() : lin(0.0), ang(0.0), stamp(0.0) {}
    };
    realtime_tools::RealtimeBuffer<Commands> command;
    Commands command_struct;
    //parameters
    std::string left_motor_joint_name,right_motor_joint_name;
    std::string right_motor_command_topic,left_motor_command_topic;
    double max_linear_velocity,min_linear_velocity;
    double max_angular_velocity,min_angular_velocity;
    double motor_distance,mass,izz;
    double right_rotational_speed_effort,left_rotational_speed_effort;
  };
  PLUGINLIB_EXPORT_CLASS(ship_controller::ShipController, controller_interface::ControllerBase);
}
#endif

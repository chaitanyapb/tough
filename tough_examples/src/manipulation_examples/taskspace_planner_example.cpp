#include <pluginlib/class_loader.h>
#include <ros/ros.h>
#include <tf/tf.h>
#include <geometry_msgs/PoseStamped.h>
#include "tough_common/tough_common_names.h"
#include "tough_moveit_planners/taskspace_planner.h"
#include "tough_controller_interface/wholebody_control_interface.h"

int main(int argc, char** argv)
{
  ros::init(argc, argv, "motion_planning_tutorial");
  ros::AsyncSpinner spinner(1);
  spinner.start();
  ros::NodeHandle nh;

  WholebodyControlInterface wb_controller(nh);
  geometry_msgs::PoseStamped pose;
  pose.pose.orientation.w = 1.0;
  pose.header.frame_id = "pelvis";

  if (argc != 4)
  {
    pose.pose.position.x = 0.4;
    pose.pose.position.y = 0.8;
    pose.pose.position.z = 0.2;
  }
  else
  {
    pose.pose.position.x = std::atof(argv[1]);
    pose.pose.position.y = std::atof(argv[2]);
    pose.pose.position.z = std::atof(argv[3]);
  }
  TaskspacePlanner man(nh);

  std::string planner_group = TOUGH_COMMON_NAMES::RIGHT_ARM_10DOF_GROUP;
  moveit_msgs::RobotTrajectory trajectory_msg;

  man.getTrajectory(pose, planner_group, trajectory_msg);
  wb_controller.executeTrajectory(trajectory_msg);
  return 0;
}

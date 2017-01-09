

// There is no status feed back from VAl only if the step is taken or not


#ifndef PASS_FOOTSTEP_HPP
#define PASS_FOOTSTEP_HPP

#include "ros/ros.h"
#include"geometry_msgs/Pose2D.h"
#include <humanoid_nav_msgs/PlanFootsteps.h>
#include "ihmc_msgs/FootstepDataListRosMessage.h"
#include "ihmc_msgs/FootstepDataRosMessage.h"
#include "ihmc_msgs/FootstepStatusRosMessage.h"
#include <tf2_ros/transform_listener.h>
#include"tf2_ros/buffer.h"
#include <geometry_msgs/TransformStamped.h>
#include "std_msgs/String.h"
#include "ros/time.h"
#include "tf/tf.h"
#include <val_common/val_common_defines.h>


class ValkyrieWalker
{

public:
    ValkyrieWalker(ros::NodeHandle nh, double InTransferTime = 1.5,double InSwingTime =1.5 , int InMode = 0);
    ~ValkyrieWalker();

    /// \todo implement this function
    bool WalkToGoal( geometry_msgs::Pose2D &goal);

    /// \todo implement this function
    bool WalkNStepsForward(int n, float x_offset = 0.4, float y_offset=0.0, bool continous = false);

    /// \todo implement this function
    bool WalkNStepsBackward(int n, float x_offset = 0.4, float y_offset=0.0, bool continous = false);

    bool WalkGivenSteps(ihmc_msgs::FootstepDataListRosMessage& list );
    void setWalkParms(float transfer_time, float swing_time, int mode);


private:
    ros::NodeHandle n;
    ros::ServiceClient footstep_client ;
    ros::Publisher footsteps_to_val ;
    ros::Subscriber footstep_status ;
    int step_counter;
    tf2_ros::Buffer tfBuffer;
    tf2_ros::TransformListener* tf_listener;
    double transfer_time,swing_time;
    int exe_mode;
    static int id ;

    std_msgs::String right_foot_frame,left_foot_frame;

    void footstepStatusCB(const ihmc_msgs::FootstepStatusRosMessage & msg);
    void getCurrentStep(int side , ihmc_msgs::FootstepDataRosMessage& foot);
    ihmc_msgs::FootstepDataRosMessage* getOffsetStep(int side, float x, float y);

    /// \todo wrong implementation. get rid of this
    void waitForSteps( int n);

    bool getFootstep(geometry_msgs::Pose2D &goal,ihmc_msgs::FootstepDataListRosMessage &list);

};
 int ValkyrieWalker::id = -1;

#endif
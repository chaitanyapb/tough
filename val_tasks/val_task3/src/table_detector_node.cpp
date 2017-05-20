#include "val_task3/table_detector.h"

int main(int argc, char** argv)
{
    ros::init (argc,argv,"table_detector");
    ros::NodeHandle nh;
    int numIterations = 0;
    bool found = false;

    geometry_msgs::Point table_location;

    table_detector detector(nh);
    //while (!found && numIterations < 20)
    while(ros::ok())
    {
        found = detector.findTable(table_location);

        numIterations++;
        ros::spinOnce();
    }

}

#include  <sstream>
#include "ros/ros.h"
#include "std_msgs/Int16.h"
#include "std_msgs/Float64.h"
#include <termio.h>
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <geometry_msgs/Pose2D.h>



using namespace std;



int main(int argc, char **argv)
{
  ros::init(argc, argv, "fake_msg");
  ros::NodeHandle nh;

  ros::Publisher gps_first_x = nh.advertise<std_msgs::Float64>("gps_x_go", 1000);
  ros::Publisher gps_first_y = nh.advertise<std_msgs::Float64>("gps_y_go", 1000);
  ros::Publisher gps_head_dir = nh.advertise<std_msgs::Float64>("gps_head_dir", 1000);
  ros::Publisher curr_pos_pub = nh.advertise<geometry_msgs::Pose2D>("curr_pose_gps", 1000);

  std_msgs::Float64 gps_x;
  std_msgs::Float64 gps_y;
  std_msgs::Float64 gps_dir;
  geometry_msgs::Pose2D curr_pos_msg;
   
     curr_pos_msg.x =20727608.794582307;
          curr_pos_msg.y =3655851.100052663;
           curr_pos_msg.theta = 270+86.2;
            

   while (ros::ok())
    {    
        // gps_x.data = 20727599.703817908 ;
        // gps_y.data =   3655953.095225785;
        // gps_first_x.publish(gps_x);
        // gps_first_y.publish(gps_y);
        // gps_dir.data = 90+86.2;
        // gps_head_dir.publish(gps_dir);
    
        curr_pos_pub.publish(curr_pos_msg);

    }
return 0;

}




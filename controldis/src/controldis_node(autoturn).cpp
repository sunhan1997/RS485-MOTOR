#include <sstream>
#include "ros/ros.h"
#include "std_msgs/Int16.h"
#include <termio.h>
#include <stdio.h>
#include<iostream>
//#include "height_border_msgs/height_border.h"

using namespace std;

	// void follow(const height_border_msgs::height_border& height_borderMsg);           

int main(int argc, char **argv)
{
  ros::init(argc, argv, "control");
  ros::NodeHandle n;

	// ros::Subscriber	sub_w = n.subscribe("/height_borderMsg",100,&follow, );
  ros::Publisher speed_pub = n.advertise<std_msgs::Int16>("speed", 1000);
  ros::Publisher turn_pub = n.advertise<std_msgs::Int16>("turn", 1000);
  ros::Publisher stop_pub = n.advertise<std_msgs::Int16>("stop", 1000);
 // ros::Publisher fake_height_pub = n.advertise<height_border_msgs::height_border>("height_border_param", 1000);
  // ros::Subscriber sub_0 = n_.subscribe("/start", 1, &start,this);
  //  int a = msg->data;
  //   if(a!=0)
    
  std_msgs::Int16 msg_speed;
  std_msgs::Int16 msg_turn;
  std_msgs::Int16 msg_stop;

  //height_border_msgs::height_border msg_fake_height;
  //直线行驶20秒
 usleep(2000000);
  msg_speed.data = 6000;
  speed_pub.publish(msg_speed);
  //左转60度
 usleep(30000000);


  msg_turn.data = -9000;
  turn_pub.publish(msg_turn);
  cout<<msg_turn.data<<endl;
  usleep(16000000);


//回到0度

  msg_turn.data = 1;
  turn_pub.publish(msg_turn);
  turn_pub.publish(msg_turn);
  turn_pub.publish(msg_turn);
    cout<<msg_turn.data<<endl;
  usleep(30000000);


   msg_speed.data = 0;
  speed_pub.publish(msg_speed);
  msg_speed.data = 0;
  speed_pub.publish(msg_speed);
  msg_speed.data = 0;
  speed_pub.publish(msg_speed);


return 0;
}

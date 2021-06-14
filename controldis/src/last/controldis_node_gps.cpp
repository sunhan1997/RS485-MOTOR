#include <sstream>
#include "ros/ros.h"
#include "std_msgs/Int16.h"
#include <termio.h>
#include <stdio.h>
#include<iostream>
using namespace std;

std_msgs::Int16 gps_x_go;
std_msgs::Int16 gps_y_go;
std_msgs::Int16 gps_x;
std_msgs::Int16 gps_y;
//go
 void xgocallback(const std_msgs::Int16::ConstPtr &msg)
  {
      gps_x_go.data = msg->data;
 }
  void ygocallback(const std_msgs::Int16::ConstPtr &msg)
  {
      gps_y_go.data = msg->data;
 }
//last
void xcallback(const std_msgs::Int16::ConstPtr &msg)
{
    gps_x.data = msg->data;
}
void ycallback(const std_msgs::Int16::ConstPtr &msg)
{
    gps_y.data = msg->data;
}

int w2pls(double W)
{
    int pls;
    if ( W< -0.03)
        pls = 0;
    else
        pls = 1000*(-0.06788*W*W*W*W+0.1948*W*W*W+0.4595*W*W+2.759*W+7.209);
    //turn = 1000*(-0.06788*W*W*W*W+0.1948*W*W*W+0.4595*W*W+2.759*W+7.209);
    if ( 0.03<W )
        pls = -1000*(-0.06788*W*W*W*W+0.1948*W*W*W+0.4595*W*W+2.759*W+7.209);
    else
        pls = 0;
    return pls;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "control");
  ros::NodeHandle n;

  ros::Publisher speed_pub = n.advertise<std_msgs::Int16>("speed", 1000);
  ros::Publisher turn_pub = n.advertise<std_msgs::Int16>("turn", 1000);
  //go
  ros::Subscriber x_go = n.subscribe("/gps_x_go", 100, xgocallback);
  ros::Subscriber y_go = n.subscribe("/gps_y_go", 100, ygocallback);
  //x y of last
  ros::Subscriber x = n.subscribe("/gps_x", 100, xcallback);
  ros::Subscriber y = n.subscribe("/gps_y", 100, ycallback);

  std_msgs::Int16 msg_speed;
  std_msgs::Int16 msg_turn;

  //v = 0.5 m/s
  ROS_INFO_STREAM("set speed:0.5m/s");
  msg_speed.data = 11000;
  speed_pub.publish(msg_speed);
  //ack
    while (ros::ok())
    {
        double err_y = gps_y.data - gps_y_go.data;
        int w = err_y*10;//P control;should change
        int pls = w2pls(w);
        msg_turn.data = pls;
        turn_pub.publish(msg_turn);
    }

return 0;
}





//*******************************开始跟踪******************//////
//
//if((angle>10)||(angle<-10))
//{
//	std_msgs::Int16 turn;
//	unsigned char rate;
//    rate = 50;
//    ros::Rate loop_rate(rate);
//    //如果车向左边偏转，那我就方向盘右转
//  if(angle>0)
//  {
//	 float output = 300*(10-angle)-1200;
//	 cout<<"motor turn"<<output<<endl;
//     turn.data = output;
//	 turn_pub.publish(turn);
//  }
//  //如果车向右边偏转，那我就方向盘左转
//  if(angle<0)
// {
//	 float output = -300*(10+angle)+1200;
//	 cout<<"motor turn"<<output<<endl;
//     turn.data = output;
//	 turn_pub.publish(turn);
// }
//  }
//
//if((dis<-5)||(dis>5))
//{
//	//如果车向左边偏转，那我就方向盘右转
//	std_msgs::Int16 turn;
//	if(dis<0)
//	{
//    float output = -dis*100+1200;
//	cout<<"motor turn"<<output<<endl;
//    turn.data = output;
//	turn_pub.publish(turn);
//	}
//     else
//     {
//	 float output = -dis * 100 -1200 ;
//	cout<<"motor turn"<<output<<endl;
//     turn.data = output;
//	turn_pub.publish(turn);
//	}
//
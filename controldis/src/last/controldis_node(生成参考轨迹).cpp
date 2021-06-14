#include <sstream>
#include "ros/ros.h"
#include "std_msgs/Int16.h"
#include "std_msgs/Float64.h"
#include <termio.h>
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <cmath>
#include<fstream>

using namespace std;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "control");
  ros::NodeHandle n;

  ros::Publisher speed_pub = n.advertise<std_msgs::Int16>("speed", 1000);
  ros::Publisher turn_pub = n.advertise<std_msgs::Int16>("turn", 1000);
//   ros::Subscriber x_go = n.subscribe("/gps_x_go", 1, xgocallback);
//   ros::Subscriber y_go = n.subscribe("/gps_y_go", 1, ygocallback);
//**********************
    time_t currentTime=time(NULL);
    char chCurrentTime[256];
    strftime(chCurrentTime,sizeof(chCurrentTime),"%Y%m%d %H%M%S",localtime(&currentTime));
    string stCurrentTime=chCurrentTime;
    string filename="line"+stCurrentTime+".txt";
    ofstream fout;
    fout.open(filename.c_str());
    double x0 = 0, y0 = 0;
    int count =0;
//**********************
  std_msgs::Int16 msg_speed;
  std_msgs::Int16 msg_turn;
  double pls;
  usleep(2000000);
  msg_speed.data = 9000;
  speed_pub.publish(msg_speed);

  double x = 21395690.323482946;

  while (x<21395723.771985769)
    {
         double x0 = x - 21395672.323482946;
         double y = 0.9635*x0-0.1513+3417737.200107955;  
         fout<<std::setiosflags(std::ios::fixed)  << std::setprecision(9)<<x<<" "<<y<<endl;
         fout<<"\r\n"<<endl;
         x = x + 0.1;

    }

return 0;
}
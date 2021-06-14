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

std_msgs::Float64 gps_x;
std_msgs::Float64 gps_y;
bool spinx = false;
bool spiny = false;

void xgocallback(const std_msgs::Float64::ConstPtr &msg)
 {
     gps_x.data = msg->data;
    //  cout<<">>y_error:"<<gps_x.data<<endl;
    //  ROS_INFO_STREAM(gps_x.data);
    //  spinx = true;
}
  void ygocallback(const std_msgs::Float64::ConstPtr &msg)
  {
      gps_y.data = msg->data;
    //   spiny = true;
 }

//int w2pls(double W)
//{
//    int pls;
//    if ( W< -0.03)
//        pls = 0;
//    else
//        pls = 1000*(-0.06788*W*W*W*W+0.1948*W*W*W+0.4595*W*W+2.759*W+7.209);
//      //turn = 1000*(-0.06788*W*W*W*W+0.1948*W*W*W+0.4595*W*W+2.759*W+7.209);
//    if ( 0.03<W )
//        pls = -1000*(-0.06788*W*W*W*W+0.1948*W*W*W+0.4595*W*W+2.759*W+7.209);
//    else
//        pls = 0;
//    return pls;
//}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "control");
  ros::NodeHandle n;

  ros::Publisher speed_pub = n.advertise<std_msgs::Int16>("speed", 1000);
  ros::Publisher turn_pub = n.advertise<std_msgs::Int16>("turn", 1000);
  ros::Subscriber x_go = n.subscribe("/gps_x_go", 1, xgocallback);
  ros::Subscriber y_go = n.subscribe("/gps_y_go", 1, ygocallback);
//**********************
    time_t currentTime=time(NULL);
    char chCurrentTime[256];
    strftime(chCurrentTime,sizeof(chCurrentTime),"%Y%m%d %H%M%S",localtime(&currentTime));
    string stCurrentTime=chCurrentTime;
    string filename="turn"+stCurrentTime+".txt";
    ofstream fout;
    fout.open(filename.c_str());
    double x0 = 0, y0 = 0;
    int count =0;
//**********************************
 ifstream readFile;
 readFile.open("/home/sunh/yammar_ws/src/controldis/data20210315 153037.txt", ios::in);
 if (!readFile.is_open())
 {
       cout << "打开文件失败" << endl;
 }
 vector<double> V;
 vector<double>::iterator it;
 double d;
 while (readFile >> d)
 V.push_back(d);//将数据压入堆栈。//
 readFile.close();
 int i = 0;
 for(it = V.begin();it != V.end();it++)
 {
   i++;
 }

  std_msgs::Int16 msg_speed;
  std_msgs::Int16 msg_turn;
  double pls;
  usleep(2000000);
  msg_speed.data = 9000;
  speed_pub.publish(msg_speed);
  speed_pub.publish(msg_speed);
   speed_pub.publish(msg_speed);
    speed_pub.publish(msg_speed);
     speed_pub.publish(msg_speed);
      speed_pub.publish(msg_speed);
       speed_pub.publish(msg_speed);

    

   int a=0; 
  while (a<i)
    {
        ros::spinOnce();
        // if(spinx == false && spiny == false)
        // {
        //     continue;
        // }
        //  double x0 = gps_x.data -21395672.323482946;
        //  double y0 = gps_y.data  -3417737.200107955;
        //  fout<<std::setiosflags(std::ios::fixed)  << std::setprecision(9)<<x0<<" "<<y0<<endl;
        //  fout<<"\r\n"<<endl;
        //  usleep(5000000);

         double x0 = V[a];
         double y0 = V[a+1];
         fout<<std::setiosflags(std::ios::fixed)  << std::setprecision(9)<<y0<<endl;
         fout<<"\r\n"<<endl;
         a = a+2;
        //  double y_err = (-(-0.9635*x0+ y0 +0.1513)/sqrt(0.9635*0.9635+1))-1.6 ;  //看看目标点和当前点的差值为正或者负
        // //  cout<<">》》》》》》》》》》》》》》》》》》》》》》》》》>y_error:"<<y_err<<endl;
        // if(y_err>0.3)
        // {
        //     pls =y_err*1400+2400; // 收割机左转，
        // }
        // if(0.1<y_err<0.3)
        // {
        //     pls = 1500; // 收割机右转，
        // }
        // if(-0.1<y_err<0.1)
        // {
        //     pls = 0; // 收割机右转，
        // }
        // if(y_err<-0.1)
        // {
        //     pls = y_err*500-2500; // 收割机右转，
        // }
        // //  cout<<">>pls:"<<pls<<endl;
        // msg_turn.data = pls;
        // double turn = -0.0001817*pls*pls*pls*0.000000001+ 0.0004183*pls*pls*0.000001  +  0.1127*pls*0.001 + 2.015;
        // fout<<std::setiosflags(std::ios::fixed)  << std::setprecision(9)<<turn<<endl;
        // fout<<"\r\n"<<endl;
        // turn_pub.publish(msg_turn);
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
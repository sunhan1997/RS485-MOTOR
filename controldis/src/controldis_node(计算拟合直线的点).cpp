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

using namespace std;




int main(int argc, char **argv)
{
   
  double x0 = 21395694.928438500 ;
  double y0=  3417756.198239216;
  double n=-40.0;
  double m=0.0;
  
  string filename="data_go.txt";
  ofstream fout;
  fout.open(filename.c_str());
  
  for(;n<30;n=n+0.01)
  {
      m = 0.9846*n-0.001607   + y0;
      double n1 = n+x0;
      cout<<m<<endl;
      fout<<std::setiosflags(std::ios::fixed)  << std::setprecision(9)<<n1<<" "<<m<<endl;
  }





//   ros::init(argc, argv, "control");
//   ros::NodeHandle n;

//   ros::Publisher speed_pub = n.advertise<std_msgs::Int16>("speed", 1000);
//   ros::Publisher turn_pub = n.advertise<std_msgs::Int16>("turn", 1000);
//   //go
//   ros::Subscriber x_go = n.subscribe("/gps_x_go", 100, xgocallback);
//   ros::Subscriber y_go = n.subscribe("/gps_y_go", 100, ygocallback);
//   ros::Subscriber head_go = n.subscribe("/gps_head_dir", 100, headgocallback);

//   //create txt to record the path
//   string filename="data_go.txt";
//   ofstream fout;
//   fout.open(filename.c_str());
//   // wait for the state
//   usleep(1000000);
//   //read the txt
//   ifstream readFile;
//   readFile.open("/home/sunh/yammar_ws/src/controldis/data9.txt", ios::in);
//   if (!readFile.is_open())
//   {
//         cout << "打开文件失败" << endl;
//   }

//   vector<double> V;
//   vector<double>::iterator it;
//   double d;

//    while (ros::ok())
//     {    
//         ros::spinOnce();
//         int currInd = find_goal_point(V,gps_x_go.data,gps_y_go.data,prevInd);
//         double theta_goal =atan((gps_y_go.data-V[currInd+1])/(gps_x_go.data-V[currInd]));
//         cout<<"<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<theta_goal:"<<theta_goal<<endl;
//         // alpha = theta_goal - 3.1415926*(270-gps_head_dir.data)/180.0;////////very important
//         alpha = theta_goal - 3.1415926*( 90-gps_head_dir.data)/180.0;////////very important
//         cout<<"<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<gps_head_dir:"<< 3.1415926*(gps_head_dir.data)/180.0<<endl;
//         cout<<"<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<alpha:"<<alpha<<endl;
//         double ld = sqrt((gps_y_go.data-V[currInd+1])*(gps_y_go.data-V[currInd+1]) + (gps_x_go.data-V[currInd])*(gps_x_go.data-V[currInd]));
//         double w =sin(alpha)/ld;        
//         cout<<"<<<<<<<<<<<<<<currInd"<<currInd<<endl;
//         cout<<"<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<w:"<<w<<endl;
//         // cout<<"<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<gps_head_dir.data:"<<gps_head_dir.data<<endl;
//         tmpDis = sqrt((V[currInd] - gps_x_go.data) * (V[currInd] - gps_x_go.data) + (V[currInd + 1] - gps_y_go.data) * (V[currInd + 1] - gps_y_go.data));
//         // if(w>0.5)
//         // {
//         //     w = 0.5;
//         // }
//         // if(w<-0.5)
//         // {
//         //     w = -0.5;
//         // }
//         int pls = w2pls(1.2*w);
//         //  fout<<gps_x_go.data<<" "<<gps_y_go.data<<" "<<w<<" "<<pls<<" "<<tmpDis<<endl;
//         // fout<<gps_x_go.data<<" "<<gps_y_go.data<<endl;
//         // fout<<"\r\n"<<endl;
//         if(pls>7000)
//         {
//             pls = 7000;
//         }
//         else if(pls<-7000)
//         {
//             pls = -7000;
//         }

//         msg_turn.data = 1.4*pls;
  
//         cout<<"<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<pls:"<<pls<<endl;

//         turn_pub.publish(msg_turn);
//         prevInd = currInd;
//     }
// return 0;
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
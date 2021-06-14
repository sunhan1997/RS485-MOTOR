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

std_msgs::Float64 gps_x_go;
std_msgs::Float64 gps_y_go;
std_msgs::Float64 gps_head_dir;
double tmpDis = 0;


//go
 void xgocallback(const std_msgs::Float64::ConstPtr &msg)
  {
      gps_x_go.data = msg->data;
 }
  void ygocallback(const std_msgs::Float64::ConstPtr &msg)
  {
      gps_y_go.data = msg->data;
 }
   void headgocallback(const std_msgs::Float64::ConstPtr &msg)
  {
      gps_head_dir.data = msg->data;
 }

// // input:
// void find_goal_point(vector<double>&vec, double x, double y, int index2){
//     int i = index2;
//     double tmpDis = 0;
//     while (vec[i] < x){
//         i = i + 2;
//     }

//     while(tmpDis < 1){
//         tmpDis = sqrt( (vec[i]-x)*(vec[i]-x)+ (vec[i+1]-y)*(vec[i+1] -y) );
//         i +=2;        
//     }
//     index1 = i;
//     // return i;
// }

int find_goal_point(vector<double>&vec, double x, double y, int prevInd){
    int i = prevInd;
    cout<<"<<<<<   prev    <<<<<<<<:"<<i<<endl;
    tmpDis = sqrt( (vec[i]-x)*(vec[i]-x)+ (vec[i+1]-y)*(vec[i+1] -y) );
    //前视距离
   while(tmpDis < 4)
    {
        tmpDis = sqrt((vec[i] - x) * (vec[i] - x) + (vec[i + 1] - y) * (vec[i + 1] - y));
        i += 2;
        }

    double x1 = x - 21395701.434035331; 
    double y1 = y - 3417762.714564383;
    cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<<<:"<<vec[i]- 21395701.434035331<<"  "<<vec[i+1]- 3417762.714564383<<endl;
    cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<<<:"<<x1<<"  "<<y1<<endl;
    cout<<"<<<<<<<<<<<<<<DIS:"<<tmpDis<<endl;
    cout<<"<<<<<   curr    <<<<<<<<:"<<i<<endl;
    return i;
}




int w2pls(double W)
{
    int pls;
    // if ( W< -0.03)
    //     pls = 1000*(-0.06788*W*W*W*W+0.1948*W*W*W+0.4595*W*W+2.759*W+7.209);
    // else if(0.03<W)
    //     pls = -1000*(-0.06788*W*W*W*W+0.1948*W*W*W+0.4595*W*W+2.759*W+7.209);
    // else
    //     pls = 0;

    // if (W<0)
    // pls = -(42.37*W-0.8415)*91.4;  
    // else
    // pls = (42.37*W-0.8415)*91.4;  //W大于0左传

    if ( W< -0.03)
        pls = -(1805*(-W)*(-W)*(-W)-778.4*(-W)*(-W)+273.5*(-W)+10.34)*91.4;
    else if(0.03<W)
        pls = (1805*W*W*W-778.4*W*W+273.5*W+10.34)*91.4;
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
  ros::Subscriber head_go = n.subscribe("/gps_head_dir", 100, headgocallback);

  //create txt to record the path
  string filename="data_go.txt";
  ofstream fout;
  fout.open(filename.c_str());
  // wait for the state
  usleep(1000000);
  //read the txt
  ifstream readFile;
  readFile.open("/home/sunh/yammar_ws/src/controldis/data3.txt", ios::in);
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

  //v = 0.5 m/s
  ROS_INFO_STREAM("set speed:0.5m/s");
  msg_speed.data = 11000;
  speed_pub.publish(msg_speed);
    usleep(500000);

   double alpha = 0;
   int prevInd = 0;
   //ack
   while (ros::ok())
    {    
        ros::spinOnce();
        int currInd = find_goal_point(V,gps_x_go.data,gps_y_go.data,prevInd);
        double theta_goal =atan((gps_y_go.data-V[currInd+1])/(gps_x_go.data-V[currInd]));
        cout<<"<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<theta_goal:"<<theta_goal<<endl;
        alpha = theta_goal - 3.1415926*(270-gps_head_dir.data)/180.0;////////very important
        cout<<"<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<gps_head_dir:"<< 3.1415926*(270-gps_head_dir.data)/180.0<<endl;
        cout<<"<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<alpha:"<<alpha<<endl;
        double ld = sqrt((gps_y_go.data-V[currInd+1])*(gps_y_go.data-V[currInd+1]) + (gps_x_go.data-V[currInd])*(gps_x_go.data-V[currInd]));
        double w =sin(alpha)/ld;        
        cout<<"<<<<<<<<<<<<<<currInd"<<currInd<<endl;
        cout<<"<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<w:"<<w<<endl;
        // cout<<"<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<gps_head_dir.data:"<<gps_head_dir.data<<endl;
        tmpDis = sqrt((V[currInd] - gps_x_go.data) * (V[currInd] - gps_x_go.data) + (V[currInd + 1] - gps_y_go.data) * (V[currInd + 1] - gps_y_go.data));
        // if(w>0.5)
        // {
        //     w = 0.5;
        // }
        // if(w<-0.5)
        // {
        //     w = -0.5;
        // }
        int pls = w2pls(1.1*w);
        //  fout<<gps_x_go.data<<" "<<gps_y_go.data<<" "<<w<<" "<<pls<<" "<<tmpDis<<endl;
        // fout<<gps_x_go.data<<" "<<gps_y_go.data<<endl;
        // fout<<"\r\n"<<endl;
        if(pls>7000)
        {
            pls = 7000;
        }
        else if(pls<-7000)
        {
            pls = -7000;
        }

        msg_turn.data = 1.37*pls;
  
        cout<<"<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<pls:"<<pls<<endl;

        turn_pub.publish(msg_turn);
        prevInd = currInd;
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
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
  usleep(20000000);
  msg_speed.data = 6000;
  speed_pub.publish(msg_speed);
  cout<<">>>>long"<<endl;
  //左转60度

//   msg_fake_height.height = 80;
//   fake_height_pub.publish(msg_fake_height);

for(int i = 1 ; i<9;i++)
{
  msg_turn.data = -1000*i;
  turn_pub.publish(msg_turn);
  cout<<msg_turn.data<<endl;
  usleep(200000);
}

  cout<<">>>>turn"<<endl;

//回到0度
for(int a = 9 ; a>-1;a--)
{
  msg_turn.data = -1000*a;
  turn_pub.publish(msg_turn);
    cout<<msg_turn.data<<endl;
  usleep(300000);
}

   msg_speed.data = 0;
  speed_pub.publish(msg_speed);




return 0;
}

// double angle ;
// double dis  ;
// void follow(const height_border_msgs::height_border& height_borderMsg)
// {
// 	string be_angle=height_borderMsg.angle_3d;
//   double angle = atof(be_angle.c_str());
// 	string be_dis=height_borderMsg.dis_2d;
//   double dis = atof(be_dis.c_str());
// 	std_msgs::Int16 speed;



//   if((angle>10)||(angle<-10))
//   {       
// 			 std_msgs::Int16 turn;
// 			 unsigned char rate;  
//              rate = 50;  
//              ros::Rate loop_rate(rate); 
//        //如果车向左边偏转，那我就方向盘右转
// 			 if(angle>0)
//             {
// 			 float output = 300*(10-angle)-1200;
// 			 cout<<"motor turn"<<output<<endl;
//              turn.data = output;
// 			 pub_turn.publish(turn);
// 			}
// 			  //如果车向右边偏转，那我就方向盘左转
// 		    if(angle<0)
//             {
// 			 float output = -300*(10+angle)+1200;
// 			 cout<<"motor turn"<<output<<endl;
//              turn.data = output;
// 			 pub_turn.publish(turn);
// 			}
//   }
//   else
//   {

// 	    if(dis>25)
// 			{
// 		 //如果车向左边偏转，那我就方向盘右转
// 	     std_msgs::Int16 turn;		
	
// 			 float output = -1000;
//        turn.data = output;
// 			 turn_pub.publish(turn);
// 			 output = -3000;
//        turn.data = output;
// 			 turn_pub.publish(turn);
//       }

// 		  if(dis>10)
// 			{
// 		 //如果车向左边偏转，那我就方向盘右转
// 	     std_msgs::Int16 turn;		
// 			 float output = -2000;
//        turn.data = output;
// 			 turn_pub.publish(turn);
//       }
// 		  if(dis>5)
// 			{
// 		 //如果车向左边偏转，那我就方向盘右转
// 	     std_msgs::Int16 turn;		
// 			 float output = 0;
//        turn.data = output;
// 			 turn_pub.publish(turn);
//       }
// 		  if(dis<5)
// 			{
// 		 //如果车向左边偏转，那我就方向盘右转
// 	     std_msgs::Int16 turn;		
// 			 float output = 500;
//        turn.data = output;
// 			turn_pub.publish(turn);
//       }

// 		//    	        if((dis<-5)||(dis>5))
// 		// 	{
// 		//  //如果车向左边偏转，那我就方向盘右转
// 	    //      std_msgs::Int16 turn;
// 	  	//      //float output = pid_run(1.5,dis);
// 		// 	if(dis<0)
// 		// 	{
// 		// 	   float output = -dis*100+1200;
// 		// 	   	cout<<"motor turn"<<output<<endl;
//         //        turn.data = output;
// 		// 	   pub_turn.publish(turn);
// 		// 	}
// 		// 	else
//         //      {
// 		// 	   float output = -dis * 100 -1200 ;
// 		// 	   cout<<"motor turn"<<output<<endl;
//         //        turn.data = output;
// 		// 	   pub_turn.publish(turn);
// 		// 	 }
		
//         //    }
	      
// //   }

//   	// if (dis<1)
// 	// {
// 	//   std_msgs::Int16 msg_stop;
// 	//   msg_stop.data = 1;
//     //   pub_stop.publish(msg_stop);
// 	// }
// }
    
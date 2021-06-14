#include <ros/ros.h>
#include <iostream>
#include <math.h>
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include "height_border_msgs/height_border.h"

using namespace std;


class sub_pub
{
private:
	ros::NodeHandle nh;
	// ros::Subscriber sub1;
	// ros::Subscriber sub2;
	// ros::Subscriber sub3;
	// ros::Subscriber sub4;
	ros::Subscriber sub_w;
	ros::Publisher pub_turn;
	ros::Publisher pub_speed;
	ros::Publisher pub_stop;

public:
	sub_pub()
	{
		// sub1 = nh.subscribe("/travel_angle", 1000, &sub_pub::Callback, this);
		// sub2 = nh.subscribe("/turn_angle", 1000, &sub_pub::Callback, this);
		// sub3 = nh.subscribe("/mainlever_angle", 1000, &sub_pub::Callback, this);
		// sub4 = nh.subscribe("/natural_angle", 1000, &sub_pub::Callback, this);
		///////
		sub_w = nh.subscribe("/height_borderMsg",100,&sub_pub::Callback, this);
		pub_turn = nh.advertise<std_msgs::Int16>("/turn",1000);
		pub_speed = nh.advertise<std_msgs::Int16>("/speed",1000);
	    pub_stop = nh.advertise<std_msgs::Int16>("/stop",1000);
	}
	void Callback(const height_border_msgs::height_border& height_borderMsg);           
};

void sub_pub::Callback(const height_border_msgs::height_border& height_borderMsg)
{
	string be_angle=height_borderMsg.angle_3d;
    double angle = atof(be_angle.c_str());
	string be_dis=height_borderMsg.dis_2d;
    double dis = atof(be_dis.c_str());
    // int tu=travel_angle->data;
    // int ml=travel_angle->data;
    // int na=natural_angle->data;
	std_msgs::Int16 speed;
	speed.data = 6000;
	pub_speed.publish(speed);

    
 
	        if(dis>25)
			{
		 //如果车向左边偏转，那我就方向盘右转
	         std_msgs::Int16 turn;		
			 float output = -500;
               turn.data = output;
			   pub_turn.publish(turn);

			   	 output = -1000;
               turn.data = output;
			   pub_turn.publish(turn);
			      	 output = -1500;
               turn.data = output;
			   pub_turn.publish(turn);
			      	 output = -2000;
               turn.data = output;
			   pub_turn.publish(turn);
		
           }
		   	        if(dis>10)
			{
		 //如果车向左边偏转，那我就方向盘右转
	         std_msgs::Int16 turn;		
			 float output = -1500;
               turn.data = output;
			   pub_turn.publish(turn);
		
           }
		   	        if(dis>5)
			{
		 //如果车向左边偏转，那我就方向盘右转
	         std_msgs::Int16 turn;		
			 float output = -500;
               turn.data = output;
			   pub_turn.publish(turn);
		
           }
		   		   	        if(dis<5)
			{
		 //如果车向左边偏转，那我就方向盘右转
	         std_msgs::Int16 turn;		
			 float output = 500;
               turn.data = output;
			   pub_turn.publish(turn);
		
           }

}
    

int main(int argc, char **argv)
{

    ros::init(argc, argv, "sub_pub1");
	pid_init();
    sub_pub sub_puber;
    ros::spin();
    return 0;
}


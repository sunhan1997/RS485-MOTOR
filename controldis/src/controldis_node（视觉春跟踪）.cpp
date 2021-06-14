#include <ros/ros.h>
#include <iostream>
#include <math.h>
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include "height_border_msgs/height_border.h"

using namespace std;

double alpha = 0;
double d = 1.5;//孙汉的视觉距离
double theta = 0;

int w2pls(double W)
{
    int pls;
    if ( W< -0.03)
        pls = -(1805*(-W)*(-W)*(-W)-778.4*(-W)*(-W)+273.5*(-W)+10.34)*91.4;
    else if(0.03<W)
        pls = (1805*W*W*W-778.4*W*W+273.5*W+10.34)*91.4;
    else
        pls = 0;
    return pls;
}


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
	speed.data = 11000;
	pub_speed.publish(speed);
    std_msgs::Int16 msg_turn;

    while (ros::ok())
    {
        // ros::spinOnce();
        double error = dis;
        theta = atan(error/d);
        alpha = theta + angle;
        double w = sin(alpha);
        cout<<"<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<w:"<<w<<endl;
        cout<<"<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<angle"<<(angle/3.1415926)*180<<endl;
        cout<<"<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<error"<<error<<endl;
        
		int pls = w2pls(1.1*w);
        if(pls>7000)
        {
            pls = 7000;
        }
        else if(pls<-7000)
        {
            pls = -7000;
        }

        msg_turn.data = pls;
        cout<<"<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<pls:"<<pls<<endl;
        pub_turn.publish(msg_turn);
    }
		 //方向盘右转 pls为-
}
    

int main(int argc, char **argv)
{

    ros::init(argc, argv, "sub_pub1");
	pid_init();
    sub_pub sub_puber;
    ros::spin();
    return 0;
}


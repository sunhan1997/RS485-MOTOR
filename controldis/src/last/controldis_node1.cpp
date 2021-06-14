#include <ros/ros.h>
#include <iostream>
#include <math.h>
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"

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
		sub_w = nh.subscribe("/dis",100,&sub_pub::Callback, this);
		pub_turn = nh.advertise<std_msgs::Int16>("/turn",1000);
		pub_speed = nh.advertise<std_msgs::Int16>("/speed",1000);
	    pub_stop = nh.advertise<std_msgs::Int16>("/stop",1000);

	}
	void Callback(const std_msgs::String::ConstPtr &msg);           
};

void sub_pub::Callback(const std_msgs::String::ConstPtr &msg)
{
	string dis_before=msg->data;
    double dis = atof(dis_before.c_str());
	

    // int tu=travel_angle->data;
    // int ml=travel_angle->data;
    // int na=natural_angle->data;
	if (1.5>dis)
	{
		cout<<"distance="<<dis<<endl;
		//cout<<">>start control"<<endl;
			//  unsigned char rate;  
            //  rate = 50;  
            //  ros::Rate loop_rate(rate); 

			//  std_msgs::Int16 speed;
	        //  speed = 1;
			//  cout<<speed<<endl;
		    //  pub2.publish(speed); 

            // //固定转向
			// std_msgs::Int64 turn;  
            // turn = output;
            // pub1.publish(turn);
		    // ros::Duration(0.5).sleep();
		    // loop_rate.sleep(); 



             std_msgs::Int16 speed;
			 std_msgs::Int16 turn;
	        //  speed.data = 9000;
		    //  pub_speed.publish(speed.data); 

			 unsigned char rate;  
             rate = 50;  
             ros::Rate loop_rate(rate); 
			 //启动路径跟踪
             float output = pid_run(1.5,dis);

			 cout<<"motor turn"<<output<<endl;
             turn.data = output;
             //pub_speed.publish(speed.data);
			 pub_turn.publish(turn);
             ros::Duration(0.5).sleep();
             loop_rate.sleep(); 
            
	}

	if (dis<1)
	{
	  std_msgs::Int16 msg_stop;
	  msg_stop.data = 1;
      pub_stop.publish(msg_stop);
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


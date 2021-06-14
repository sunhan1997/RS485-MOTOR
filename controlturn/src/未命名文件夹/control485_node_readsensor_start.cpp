//
// Created by sjtu_wanghaili on 2020/6/4.
//

#include<iostream>
#include<string>
#include<unistd.h>  /* UNIX standard function definitions */
#include<fcntl.h>   /* File control definitions */
#include<termios.h> /* POSIX terminal control definitions */
#include<sys/time.h>    //for time
#include<modbus.h>
#include<fstream>
#include<vector>
#include<algorithm>
#include<signal.h>
#include<pthread.h>

#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include "std_msgs/Float32.h"

using namespace std;






/////////////////********************************SUNHAN
modbus_t* com;//com用于电机速度控制反馈
uint16_t HUNDLE=0x02; //HUNDLE
uint16_t MAINLEVER=0x01; //MAINLEVER
uint16_t startstop=0x03E8;//ID30 启动/关闭电机
uint16_t controlmod=0x03E9;//ID31 位置控制模式
uint16_t seta=0x03ED;//ID34 加速度
uint16_t setaa=0x03EE;//ID35 减速度
uint16_t posezero=0x03EA;//ID32  上位2字节
uint16_t posezero1=0x03EB;//ID32  下位2字节
uint16_t motorpos1=0x03EF; //控制位置，前两字节
uint16_t motorpos2=0x03F0; //控制位置，后两字
uint16_t readpose=0x07D0; //读取位置消息

std_msgs::Float32 angle_turn1;
std_msgs::Float32 angle_speed1;
/////////////////********************************SUNHAN


string port="/dev/ttyUSB0";
bool openSerial(const char* port);
//string getTime(void);//获取当前系统时间
void SetModbus(int motor1);
void movemotor(int motor1,int speed);

void controlset(int motor1);
void stopmotor(int motor1);
int motorReadSpeed(int motor);



bool openSerial(const char* port)
{
    com=modbus_new_rtu(port,19200,'E',8,1);
    if(com==nullptr)
    {
        cout<<"wrong modbus parameter."<<endl;
        return false;
    }
    timeval time_out;
    time_out.tv_sec=0;
    time_out.tv_usec=1000*100;
   // modbus_set_response_timeout(com,time_out.tv_sec,time_out.tv_usec);
    modbus_rtu_set_serial_mode(com,MODBUS_RTU_RS485);
    if(modbus_connect(com)==-1)
    {
        cout<<"Cannot connect modbus at port:"<<port<<endl;
        return false;
    } else
        cout<<"Connected modbus at port:"<<port<<endl;
    return true;
}


///////////////////////////************电机伺服打开*************//////////////////
void SetModbus(int motor1)
{
    const uint16_t src[]={0x1};
    modbus_set_slave(com,motor1); 
    modbus_write_registers(com,startstop,1,src);
}
//****************************************************************************************
///////////////////////////************HUNDLE电机位置控制*************//////////////////
void controlset(int motor1)
{
    
    const uint16_t src[]={0x01};
    const uint16_t srcc[]={0x64};
    const uint16_t posez[]={0x0}; 
    modbus_set_slave(com,motor1); 
    modbus_write_registers(com,controlmod,1,src);  //设置控制模式---位置控制 
    modbus_write_registers(com,seta,1,srcc);       //设置加速度
    modbus_write_registers(com,setaa,1,srcc);      //设置减速度
    // modbus_write_registers(com,posezero,1,posez);
    //modbus_write_registers(com,posezero1,1,posez);
    cout<<"motor INIT!!!!!!!"<<endl;
}
//****************************************************************************************
///////////////////////////************HUNDLE电机控制*************//////////////////
void movemotor(int motor1,int speed)
{
    if(speed>0)
    {
    //左转&&向前？？？？？？
    uint16_t src[]={0X00};
    uint16_t srcc[]= {(uint16_t)speed};
    cout<<*srcc<<endl;
    //uint16_t srcc[]= {0x00};
    modbus_set_slave(com,motor1); 
    modbus_write_registers(com,motorpos1,1,src);
    modbus_write_registers(com,motorpos2,1,srcc);
    }
    else
    {
    //右转&&向后？？？？？？
    //如果speed = -100,那么speed = 0XFFFF-speed=65535-100=65435,不知道具体输入到电机是什么情况
    //可以向将一个10进制数输入到srcc里面看看情况。
    speed =-speed;
    speed = 65535-speed;
     uint16_t srrc[]={0XFFFF};
    uint16_t ssrcc[]= {(uint16_t)speed};
    modbus_set_slave(com,motor1); 
    modbus_write_registers(com,motorpos1,1,srrc);
    modbus_write_registers(com,motorpos2,1,ssrcc);
    }
   
}

//****************************************************************************************

///////////////////////////************电机关机*************//////////////////
void stopmotor(int motor1)
{
    
    const uint16_t src[]={0x0};
    modbus_set_slave(com,motor1); 
    modbus_write_registers(com,startstop,1,src);
    cout<<"motor STOP!!!!!!!!"<<endl;
}
//****************************************************************************************
///////////////////////////************电机位置读取*************//////////////////
int motorReadSpeed(int motor)
{
    uint16_t temp[64];
    modbus_set_slave(com,motor);
    int flag=modbus_read_registers(com,readpose,2,temp);
    usleep(2000);
    if(flag==-1)
    {
        cout<<"error read motor:"<<motor<<" speed."<<endl;
    } else{
        cout<<"succeed read motor"<<motor<<" speed."<<endl;
    }
    cout<<temp<<endl;

// uint16_t tab_reg[64];
// int rc;
// int i;

//  modbus_set_slave(com,motor);
// if (modbus_connect(com) == -1) {
//     fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
//     modbus_free(com);
//     return -1;
// }
 
// rc = modbus_read_registers(com, readpose, 2, tab_reg);
// if (rc == -1) {
//     fprintf(stderr, "%s\n", modbus_strerror(errno));
//     return -1;
// }
 
// for (i=0; i < rc; i++) {
//     printf("reg[%d]=%d (0x%X)\n", i, tab_reg[i], tab_reg[i]);
// }
}
//****************************************************************************************



class sub_pub
{
private:
	ros::NodeHandle n_;
    ros::Subscriber sub_0;
    ros::Subscriber sub_;
    ros::Subscriber sub1_;
    ros::Subscriber sub2_;
    ros::Subscriber sub3_;
    ros::Subscriber sub4_;

public:
	sub_pub()
	{
        ///*****************new
        sub_0 = n_.subscribe("/start", 1, &sub_pub::start,this);
        ///*****************new
		sub_ = n_.subscribe("/speed", 1, &sub_pub::speed,this);
        sub1_ = n_.subscribe("/turn", 1, &sub_pub::turn,this);
        sub2_ = n_.subscribe("/stop", 1, &sub_pub::stop,this);
        sub3_ = n_.subscribe("/angle_turn", 1, &sub_pub::angle_turn,this);
        sub4_ = n_.subscribe("/angle_speed", 1, &sub_pub::angle_speed,this);
	}
       
        void angle_turn(const std_msgs::Float32::ConstPtr &msg);
        void angle_speed(const std_msgs::Float32::ConstPtr &msg);

        void start(const std_msgs::Int16::ConstPtr &msg);
                ///*****************new
        void speed(const std_msgs::Int16::ConstPtr &msg);
        void turn(const std_msgs::Int16::ConstPtr &msg);
        void stop(const std_msgs::Int16::ConstPtr &msg);         
};





void sub_pub::angle_turn(const std_msgs::Float32::ConstPtr &msg)
 {
     angle_turn1.data = msg->data;

}
void sub_pub::angle_speed(const std_msgs::Float32::ConstPtr &msg)
 {
     angle_speed1.data = msg->data;

}


void sub_pub::start(const std_msgs::Int16::ConstPtr &msg)
 {
    int a = msg->data;
      if(a != 0 && 1800 < angle_turn1.data && angle_turn1.data<2100 && 2200<angle_speed1.data && angle_speed1.data<2400)
    //if(a!=0)
    {
            cout<<"start"<<endl;

    SetModbus(MAINLEVER);//电机打开
    controlset(MAINLEVER);//位置控制，加速度减速度100,位置回0点..
    SetModbus(HUNDLE);//电机打开
    controlset(HUNDLE);//位置控制，加速度减速度100,位置回0点..
    }

}

void sub_pub::speed(const std_msgs::Int16::ConstPtr &msg)
 {
    ROS_INFO_STREAM("carspeed: "<<msg->data);
    int v = msg->data;
    movemotor(MAINLEVER,v);
}

int i = 0;

void sub_pub::turn(const std_msgs::Int16::ConstPtr &msg)
 {
    ROS_INFO_STREAM("carturn: "<<msg->data);
    int w = msg->data;
    // if(w>i)
    // {
    //     for( ;i<w;)
    //   {
    //      movemotor(HUNDLE,i);
    //       i = i + 100;
    //   }
    // }

    //  if(w<i)
    // {
    //     for(;i>w;)
    // {
     movemotor(HUNDLE,w);
    //  i = i - 100;
    // }
    // }    

    
}

void sub_pub::stop(const std_msgs::Int16::ConstPtr &msg)
 {   
      int a = msg->data;
      cout<<"stop"<<endl;
      if(a != 0)
    {
     movemotor(MAINLEVER,0);
     movemotor(HUNDLE,1);
     stopmotor(HUNDLE);
     stopmotor(MAINLEVER);
    }
}
        
	

int main (int argc, char **argv)
{
    
    ros::init(argc, argv, "sub_pub");
    sub_pub sub_puber;

    ROS_INFO_STREAM("Hello, ROS!") ;
    cout<<"usage sudo ./motor"<<endl;

    openSerial(port.c_str());
    // SetModbus(MAINLEVER);//电机打开
    // controlset(MAINLEVER);//位置控制，加速度减速度100,位置回0点..
    //  SetModbus(HUNDLE);//电机打开
    // controlset(HUNDLE);//位置控制，加速度减速度100,位置回0点..
    ros::spin();
    return 0;
}



// int main (int argc, char **argv)
// {
//     ros::init(argc, argv, "hello") ;
//     ros::NodeHandle nh;
//     ROS_INFO_STREAM("Hello, ROS!") ;
//     ros::NodeHandle n_;
//     ros::Subscriber sub_;
//     ros::Subscriber sub1_;
//     ros::Subscriber sub2_;
//     cout<<"usage sudo ./motor"<<endl;

//     // sub_ = n_.subscribe("speed", 1, &speed);
//     // sub1_ = n_.subscribe("turn", 1, &turn);
//     // sub2_ = n_.subscribe("stop", 1, &stop);
//     openSerial(port.c_str());
//     SetModbus(MAINLEVER);//电机打开
//     controlset(MAINLEVER);//位置控制，加速度减速度100,位置回0点
//     // movemotor(MAINLEVER,1000);

// while(ros::ok())
// {
//     int mingling = 0;
//     int readpose = 0;
//     cout<<"请输入控制代码："<<endl;
//     cin>>mingling;
//     // ros::init(argc, argv, "sub_pub");
//     // sub_pub sub_puber;
//     //SetModbus(HUNDLE);//电机打开
//    // controlset(HUNDLE);//位置控制，加速度减速度100,位置回0点..
//     movemotor(MAINLEVER,mingling);
// }
//     ros::spin();
//     return 0;
// }
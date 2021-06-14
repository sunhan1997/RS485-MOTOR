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
#include "std_msgs/Int32.h"
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
/////////////////********************************SUNHAN


string port="/dev/ttyUSB0";
bool openSerial(const char* port);
//string getTime(void);//获取当前系统时间
void turnInit(void);
void SetModbus(int motor1);
void moveHUNDLE(int motor1);
void moveHUNDLE1(int motor1);
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
;

///////////////////////////************启动电机*************//////////////////
void turnInit(void)
{
    SetModbus(MAINLEVER);
   // SetModbus(HUNDLE);
}
///////////////////////////************电机伺服打开*************//////////////////
void SetModbus(int motor1)
{
    
    const uint16_t src[]={0x1};
    modbus_set_slave(com,motor1); 
    modbus_write_registers(com,startstop,1,src);
    cout<<"motor start!!!!!!!!"<<endl;
}
//****************************************************************************************
///////////////////////////************HUNDLE电机位置控制*************//////////////////
void controlset(int motor1)
{
    
    const uint16_t src[]={0x01};
    const uint16_t srcc[]={0x64};
    const uint16_t posez[]={0x0};
    modbus_set_slave(com,motor1); 
    modbus_write_registers(com,controlmod,1,src);
    modbus_write_registers(com,seta,1,srcc);
    modbus_write_registers(com,setaa,1,srcc);
  // modbus_write_registers(com,posezero,1,posez);
    usleep(1000);
    usleep(1000);
    //modbus_write_registers(com,posezero1,1,posez);
    usleep(1000);
    usleep(1000);
    cout<<"motor start!!!!!!!!"<<endl;
}
//****************************************************************************************
///////////////////////////************HUNDLE电机控制*************//////////////////
void moveHUNDLE(int motor1)
{
   
    const uint16_t src[]= {0x00};
    const uint16_t srcc[]={0x1000};
    modbus_set_slave(com,motor1); 
    modbus_write_registers(com,motorpos1,1,src);
    modbus_write_registers(com,motorpos2,1,srcc);
    cout<<"motor start!!!!!!!!"<<endl;
}

void moveHUNDLE1(int motor1)
{
   
    const uint16_t src[]= {0x00};
    const uint16_t srcc[]={0x000};
    modbus_set_slave(com,motor1); 
    modbus_write_registers(com,motorpos1,1,src);
    modbus_write_registers(com,motorpos2,1,srcc);
    cout<<"motor start!!!!!!!!"<<endl;
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
int main (int argc, char **argv)
{
    ros::init(argc, argv, "hello") ;
    ros::NodeHandle nh;
    ROS_INFO_STREAM("Hello, ROS!") ;
    ros::NodeHandle n_;
    ros::Subscriber sub_;
    ros::Subscriber sub2_;
    ros::Subscriber sub3_;
    ros::Publisher pub_;
    cout<<"usage sudo ./motor"<<endl;

    int mingling = 0;
    int readpose = 0;
    openSerial(port.c_str());
    cout<<"请输入控制代码："<<endl;
    cin>>mingling;

    if (mingling == 1){
    turnInit();//电机打开
    controlset(HUNDLE);//位置控制，加速度减速度100,位置回0点
    }

// while (ros::ok())
// { 
    

    moveHUNDLE(HUNDLE);
  
    ros::Rate(100).sleep();
        moveHUNDLE1(HUNDLE);
//}

//    else{
//     stopmotor(HUNDLE);
//     stopmotor(MAINLEVER);
//    }

//  while (ros::ok())
// {   
//      motorReadSpeed(HUNDLE);
// }
    

}

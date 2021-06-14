      #include <ros/ros.h>
      #include <serial/serial.h>  //ROS已经内置了的串口包
      #include <std_msgs/String.h>
      #include <std_msgs/Empty.h>
      #include <string>
      #include <queue>
      #include <vector>
      #include <assert.h>
      #include <stdio.h>
      #include "std_msgs/Int16.h"
      #include "std_msgs/Float64.h"
      #include <fstream>

      serial::Serial ser; //声明串口对象
      using namespace std;
      //回调函数
      void write_callback(const std_msgs::String::ConstPtr& msg)
      {
      ROS_INFO_STREAM("Writing to serial port" <<msg->data);
      ser.write(msg->data);   //发送串口数据
      }

      double str2double(string str)
      {
          int size=str.size();
          if(size==0)
          {
            return 0;
          }
              int pos=0;
              double res=0.0;
              for(int i=0;i<size;i++)
              {
                  if(str[i]=='.')
                  {
                      pos=i;
                      break;
                  }
              }
              for(int i=0;i<size;i++)
              {
                  if(i<pos)
                  {
                      res+=(str[i]-'0')*pow(10,pos-i-1);
                  }
                  if(i>pos)
                  {
                      res+=(str[i]-'0')*pow(10,pos-i);
                  }
               }
            return res;
      }

    
      void RecePro(std::string s , double& lat , double& lon)
      {
          //分割有效数据，存入vector中
          std::vector<std::string> v;
          std::string::size_type pos1, pos2;
          pos2 = s.find(",");
          pos1 = 0;
          while ( std::string::npos !=pos2 )
          {
              v.push_back( s.substr( pos1, pos2-pos1 ) );
              pos1 = pos2 + 1;
              pos2 = s.find(",",pos1);
          }
          if ( pos1 != s.length() )
              v.push_back( s.substr( pos1 ));
          //解析出经纬度
          if (v.max_size() >= 6 && (v[6] == "1" || v[6] == "2" || v[6] == "3" || v[6] == "4" || v[6] == "5" || v[6] == "6" || v[6] == "8" || v[6] == "9"))
          {
              //纬度
              if (v[3] != "") lat = std::atof(v[2].c_str()) / 100;
              int ilat = (int)floor(lat) % 100;
              lat = ilat + (lat - ilat) * 100 / 60;
              //经度
              if (v[5] != "") lon = std::atof(v[4].c_str()) / 100;
              int ilon = (int)floor(lon) % 1000;
              lon = ilon + (lon - ilon) * 100 / 60;
          }
      }


      void RecePro_head(std::string s , double& head)
      {
         std::vector<std::string> v;
          //分割有效数据，存入vector中        
         std::string::size_type pos1, pos2;
          pos2 = s.find(",");
          pos1 = 0;
          while ( std::string::npos !=pos2 )
          {
              v.push_back( s.substr( pos1, pos2-pos1 ) );
              pos1 = pos2 + 1;
              pos2 = s.find(",",pos1);
          }

          if ( pos1 != s.length() )
              v.push_back( s.substr( pos1 ));
  
          if (v[2] != "") head = std::atof(v[1].c_str());
    }


  std::vector<string> split(const string& str, const string& delim)
 {
    std::vector<string> res;
    if("" == str) return res;
    //先将要切割的字符串从string类型转换为char*类型
    char * strs = new char[str.length() + 1] ; //不要忘了
    strcpy(strs, str.c_str());

    char * d = new char[delim.length() + 1];
     strcpy(d, delim.c_str());

    char *p = strtok(strs, d);
    while(p) {
        string s = p; //分割得到的字符串转换为string类型
        res.push_back(s); //存入结果数组
        p = strtok(NULL, d);
 }
 }

    ///////////////////////////////////转为XY坐标
void gaussBLtoXY(double longitude,double latitude, double &X, double &Y)
 {
    int ProjNo = 0;
    // 带宽
    int ZoneWide = 6;
    double longitude1, latitude1, longitude0, X0, Y0, xval, yval;
    double a, f, e2, ee, NN, T, C, A, M, iPI;
    // 3.1415926535898/180.0;
    iPI = 0.0174532925199433;
    // 54年北京坐标系参数
    a = 6378245.0;
    f = 1.0 / 298.3;
    // 80年西安坐标系参数
    // a=6378140.0;
    // f=1/298.257;
    ProjNo = (int) (longitude / ZoneWide);
    longitude0 = ProjNo * ZoneWide + ZoneWide / 2;
    longitude0 = longitude0 * iPI;
    // 经度转换为弧度
    longitude1 = longitude * iPI;
    // 纬度转换为弧度
    latitude1 = latitude * iPI;
    e2 = 2 * f - f * f;
    ee = e2 * (1.0 - e2);
    NN = a / sqrt(1.0 - e2 * sin(latitude1) * sin(latitude1));
    T = tan(latitude1) * tan(latitude1);
    C = ee * cos(latitude1) * cos(latitude1);
    A = (longitude1 - longitude0) * cos(latitude1);
    M = a * ((1 - e2 / 4 - 3 * e2 * e2 / 64 - 5 * e2 * e2 * e2 / 256) * latitude1 - (3 * e2 / 8 + 3 * e2 * e2 / 32 + 45 * e2 * e2 * e2 / 1024) * sin(2 * latitude1) + (15 * e2 * e2 / 256 + 45 * e2 * e2 * e2 / 1024) * sin(4 * latitude1) - (35 * e2 * e2 * e2 / 3072) * sin(6 * latitude1));
    xval = NN * (A + (1 - T + C) * A * A * A / 6 + (5 - 18 * T + T * T + 72 * C - 58 * ee) * A * A * A * A * A / 120);
    yval = M + NN * tan(latitude1) * (A * A / 2 + (5 - T + 9 * C + 4 * C * C) * A * A * A * A / 24 + (61 - 58 * T + T * T + 600 * C - 330 * ee) * A * A * A * A * A * A / 720);
    X0 = 1000000 * (ProjNo + 1) + 500000;
    Y0 = 0;
    xval = xval + X0;
    yval = yval + Y0;
    X = xval;
    Y = yval;
    return ;
}

    int main (int argc, char** argv) 
    {
          //初始化节点
          ros::init(argc, argv, "serial_example_node");
          //声明节点句柄
          ros::NodeHandle nh;
          std_msgs::Float64 gps_x;
          std_msgs::Float64 gps_y;
          std_msgs::Float64 gps_dir;


          //发布主题
          ros::Publisher gps_first_x = nh.advertise<std_msgs::Float64>("gps_x_go", 1000);
          ros::Publisher gps_first_y = nh.advertise<std_msgs::Float64>("gps_y_go", 1000);
          ros::Publisher gps_head_dir = nh.advertise<std_msgs::Float64>("gps_head_dir", 1000);

          try {
              //设置串口属性，并打开串口
              ser.setPort("/dev/ttyUSB1");
              ser.setBaudrate(115200);
              serial::Timeout to = serial::Timeout::simpleTimeout(1000);
              ser.setTimeout(to);
              ser.open();
          }
          catch (serial::IOException &e) {
              ROS_ERROR_STREAM("Unable to open port ");
              return -1;
          }
          //检测串口是否已经打开，并给出提示信息
          if (ser.isOpen()) {
              ROS_INFO_STREAM("Serial Port initialized");
          } else {
              return -1;
          }
          //指定循环的频率
          ros::Rate loop_rate(50);


          time_t currentTime=time(NULL);
          char chCurrentTime[256];
          strftime(chCurrentTime,sizeof(chCurrentTime),"%Y%m%d %H%M%S",localtime(&currentTime));
          string stCurrentTime=chCurrentTime;
          string filename="data"+stCurrentTime+".txt";
          string filename_head="head_dir"+stCurrentTime+".txt";
          ofstream fout_head;
          ofstream fout;
          fout_head.open(filename_head.c_str());
          fout.open(filename.c_str());
          double x0 = 0, y0 = 0;
          int count =0;

          std::string strRece;
          std::string strRece_h;


          while (ros::ok()) {

              if (ser.available()) {
                  //1.读取串口信息：
                  ROS_INFO_STREAM("Reading from serial port\n");
                  strRece += ser.read(ser.available());
                  //2.截取数据、解析数据：  GPS起始标志
                  std::string gstart = "$GNRMC";
                  //GPS终止标志
                  std::string gend = "\r\n";
                  int i = 0, start = -1, end = -1;
                  while (i < strRece.length()) {
                      //找起始标志
                      start = strRece.find(gstart);
                      //如果没找到，丢弃这部分数据，但要留下最后2位,避免遗漏掉起始标志
                      if (start == -1) {
                          if (strRece.length() > 2)
                              strRece = strRece.substr(strRece.length() - 3);
                          break;
                      }
                      //如果找到了起始标志，开始找终止标志
                      else {
                          //找终止标志
                          end = strRece.find(gend);
                          //如果没找到，把起始标志开始的数据留下，前面的数据丢弃，然后跳出循环
                          if (end == -1 || end < start) {
                              if (end != 0)
                                  strRece = strRece.substr(start);
                              break;
                          }
                          //如果找到了终止标志，把这段有效的数据剪切给解析的函数，剩下的继续开始寻找
                          else {
                              i = end;
                              //把有效的数据给解析的函数以获取经纬度
                              double lat, lon;
                              double x,y;
                              RecePro(strRece.substr(start, end + 2 - start), lat, lon);
                              gaussBLtoXY(lon, lat, x, y);
                                // usleep(100000);
                                cout<<x<<endl;
                                cout<<y<<endl;
                                fout<<std::setiosflags(std::ios::fixed)  << std::setprecision(9)<<x<<" "<<y<<endl;
                                fout<<"\r\n"<<endl;
                                gps_x.data = x ;
                                gps_y.data = y ;     
                                gps_first_x.publish(gps_x);
                                gps_first_y.publish(gps_y);    
                              //如果剩下的字符大于等于4，则继续循环寻找有效数据,如果所剩字符小于等于3则跳出循环
                              if (i + 5 < strRece.length())
                                  strRece = strRece.substr(end + 2);
                              else {
                                  strRece = strRece.substr(end + 2);
                                  break;
                              }
                          }
                      }
                  }



                  std::string gstart_head = "$GNHDT";
                  std::string gend_head = "\r\n";
                  int ii = 0, start_h = -1, end_h = -1;
                  while (ii < strRece_h.length()) {
                      start_h = strRece_h.find(gstart_head);
                      if (start_h == -1) {
                          if (strRece_h.length() > 2)
                              strRece_h = strRece_h.substr(strRece_h.length() - 3);
                          break;
                      }
                      else {
                          end_h = strRece_h.find(gend_head);
                          if (end_h == -1 || end_h < start_h) {
                              if (end_h != 0)
                                  strRece_h = strRece_h.substr(start_h);
                              break;
                          }
                          //如果找到了终止标志，把这段有效的数据剪切给解析的函数，剩下的继续开始寻找
                          else {
                              ii = end_h;
                              //把有效的数据给解析的函数以获取经纬度
                              double dir;
                               RecePro_head(strRece_h.substr(start_h, end_h + 2 - start_h),dir);
                            //    string gh = strRece_h.substr(start_h, end_h + 2 - start_h);
                                cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<dir<<endl;
                              fout_head<<dir<<endl;
                              fout_head<<"\r\n"<<endl;
                              gps_dir.data = dir;
                              gps_head_dir.publish(gps_dir);
                              if (ii + 5 < strRece_h.length())
                                  strRece_h = strRece_h.substr(end_h + 2);
                              else {
                                  strRece_h = strRece_h.substr(end_h + 2);
                                  break;
                                  }
                            }
                        }
                    }
                  //处理ROS的信息，比如订阅消息,并调用回调函数
                  ros::spinOnce();
                  loop_rate.sleep();
              }
          }
      }

















#include<iostream>
#include<fstream>
#include<string>        //for string
#include<time.h>        //for time
#include<sys/time.h>    //for time
#include<vector>        //for vector
#include<algorithm>     //for pow
#include<unistd.h>  /* UNIX standard function definitions */
#include<fcntl.h>   /* File control definitions */
#include<termios.h> /* POSIX terminal control definitions */
struct gps_info
{
    bool valid=false;
    double lat=0.0;//维度
    double lon=0.0;//精度
    double speed=0.0;
    double dir=0.0;
};
using namespace std;
int openSerial(char* port);//open and initialize serial port
string getTime(void);//get system clock time
bool gpsDecode(char* input,gps_info& decode);
bool gpsDecode(string,gps_info& decode);
//true for GNRMC,otherwise not GNRMC;input for gps message;decode for decode message of it.
double str2double(string);
//2019/06/12 已验证string->double的准确性
bool gpsDecodeTxt(string);
int buffSize=512;
int main(int argc,char** argv)
{ 
    gpsDecodeTxt("gps1.txt");
    return 0;
}
int openSerial(char* port)
{
    int com;
    com=open(port,O_RDWR);
    string time=getTime();
    if(com!=-1)
    {
        termios options;//参数结构体
        tcgetattr(com, &options);//获取当前参数
        //波特率9600
        options.c_ispeed=B9600;
        options.c_ospeed=B9600;
        //8数据位 无校验 一个停止位
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS8;
        tcsetattr(com, TCSANOW, &options);
        cout<<"SerialPort open success."<<endl;
    }
    else 
    {
        cout<<"SerialPort open failure."<<endl;
    }
    return com;
}
string getTime(void)
{
    timeval tv; 
	time_t timep;
    tm* timeNow;
	gettimeofday(&tv, NULL);//获取当下精确的s和us的时间
	time(&timep);//获取从1900年至今的秒数
	timeNow = gmtime(&timep); //注意tm_year和tm_mon的转换后才是实际的时间
	timeNow->tm_year+=1900;//实际年
	timeNow->tm_mon+=1;//实际月
    timeNow->tm_hour+=8;//实际小时
	long int ms = (tv.tv_sec*1000.0 + tv.tv_usec / 1000.0) - timep * 1000.0; //实际ms
    string res="";
    res+='[';
    res+=to_string(timeNow->tm_year);
    res+=' ';
    res+=to_string(timeNow->tm_mon);
    res+=' ';
    res+=to_string(timeNow->tm_mday);
    res+=' ';
    res+=to_string(timeNow->tm_hour);
    res+=':';
    res+=to_string(timeNow->tm_min);
    res+=':';
    res+=to_string(timeNow->tm_sec);
    res+=':';
    res+=to_string(int(ms));
    res+=']';
    return res;
}
bool gpsDecode(char* str,gps_info& info)
{
    if(str[1]=='G'&&str[2]=='N'&&str[3]=='R'&&str[4]=='M'&&str[5]=='C')
    {
        //根据NMEA分割消息
        vector<string> s;
        string temp;
        int cnt;
        while(*str!='\0')
        {
            if(s.size()==14)
            {
                break;
            }
            if(*str==',')
            {
                s.push_back(temp);
                temp.clear();
            }
            else
            {
                temp+=*str;
            }
            str++; 
        }
        if(s[2]=="A")//<2>定位有效位 A有效 V无效
        {
            info.valid=true;
            double lat=str2double(s[3])/100;//纬度值
            int lat_h=lat;//h
            double lat_m=(lat-lat_h)*100;
            lat=lat_h+lat_m/60;
            if(s[4]=="N")//纬度方向
            {
                info.lat=lat;
            }
            else
            {
                info.lat=-lat;
            }
            double lon=str2double(s[5])/100;//经度值
            int lon_h=lon;
            double lon_m=(lon-lon_h)*100;
            lon=lon_h+lon_m/60;
            if(s[6]=="E")//经度方向
            {
                info.lon=lon;
            }
            else
            {
                info.lon=-lon;
            }
            info.speed=str2double(s[7]);//速率值
            info.dir=str2double(s[8]);//速度方向
        }
        else
        {
            info.valid=false;
        }
        
        return true;  
        }
    else 
    {
        return false;
    }
}
bool gpsDecode(string str,gps_info& info)
{
    if(str[1]=='G'&&str[2]=='N'&&str[3]=='R'&&str[4]=='M'&&str[5]=='C')
    {
        //根据NMEA分割消息
        vector<string> s;
        string temp;
        int cnt;
        int size=str.size();
        for(int i=0;i<size;i++)
        {
            if(s.size()==14)
            {
                break;
            }
            if(str[i]==',')
            {
                s.push_back(temp);
                temp.clear();
            }
            else
            {
                temp+=str[i];
            }
        }
        if(s[2]=="A")//<2>定位有效位 A有效 V无效
        {
            info.valid=true;
            double lat=str2double(s[3])/100;//纬度值
            int lat_h=lat;//h
            double lat_m=(lat-lat_h)*100;
            lat=lat_h+lat_m/60;
            if(s[4]=="N")//纬度方向
            {
                info.lat=lat;
            }
            else
            {
                info.lat=-lat;
            }
            double lon=str2double(s[5])/100;//经度值

            if(s[6]=="E")//经度方向
            {
                info.lon=lon;
            }
            else
            {
                info.lon=-lon;
            }
            info.speed=str2double(s[7]);//速率值
            info.dir=str2double(s[8]);//速度方向
        }
        else
        {
            info.valid=false;
        }
        
        return true;  
        }
    else 
    {
        return false;
    }
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
bool gpsDecodeTxt(string inTxt)
{
    ifstream infile(inTxt);
    if(!infile.is_open())
    {
        cout<<inTxt<<"doesn't find."<<endl;
    }
    string s;
    cout<<"Decoding."<<endl;
    while(getline(infile,s))
    {
        int size=s.size();
        string time;
        string gps;
        for(int i=0;i<size;i++)
        {
            time+=s[i];
            if(s[i]==']')
            {
                for(int j=i+1;j<size;j++)
                {
                    gps+=s[j];
                }
                break;
            }
        }
        gps_info info;
        if(gpsDecode(gps,info))
        {
            string outTxt;
            outTxt+=inTxt;
            int size=outTxt.size();
            outTxt=outTxt.substr(0,size-4);
            outTxt+="_decode.txt";
            ofstream outFile(outTxt,ios_base::app);
            //cout.setf(ios_base::fixed);
            //cout.precision(6);
            outFile.setf(ios_base::fixed);
            outFile.precision(6);
            //cout<<time<<" lat="<<info.lat<<", lon="<<info.lon<<", speed="<<info.speed<<
            //", direction="<<info.dir<<endl;
            outFile<<time<<" lat="<<info.lat<<", lon="<<info.lon<<", speed="<<info.speed<<
            ", direction="<<info.dir<<endl;
            outFile.close();
        }
        if(s.empty())
        {
            cout<<"Process end."<<endl;
            infile.close();
            break;
        }
    }
    cout<<"Decode success."<<endl;
    return true;
}
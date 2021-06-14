#ifndef _MOTOR_H_
#define _MOTOR_H_
class Motor
{
private:
    int id;
    int speed;
    bool direction;
    double current;
public:
    bool setSpeed(int speed_val);
    bool setDirection(bool direction);
};

#endif
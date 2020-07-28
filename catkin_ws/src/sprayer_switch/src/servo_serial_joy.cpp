#include <ros/ros.h>
#include <serial/serial.h>
#include <iostream>
#include <string>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Bool.h>

using namespace std;

string cmd;
serial::Serial ser;

void cb(const std_msgs::Bool& spray_on)
{
  ROS_INFO("Suscribe");
  if(spray_on.data == true)
  {
    cmd = "s";
  }
  else
  {
    cmd = "f";
  }

  ser.write(cmd);
  cout <<  cmd << endl;
  sleep(1);
  while (ser.available() > 0)
    {
      string msg = ser.read(ser.available());
      ROS_INFO("%s", msg.c_str());
    }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "servo_joy");
  ros::NodeHandle n;
//  serial::Serial ser;

  string port;
  int baud;
//  string cmd;

  if (!n.getParam("baud", baud)) baud = 57600;
  if (!n.getParam("port", port)) port = "/dev/ttyACM1";
  ROS_INFO("baudrate: %d,  port: %s", baud, port.c_str());

  try
  {
    ser.setPort(port);
    ser.setBaudrate(baud);
    serial::Timeout tout = serial::Timeout::simpleTimeout(1000);
    ser.setTimeout(tout);
    ser.open();
  }
  catch(serial::IOException& e)
  {
    ROS_ERROR_STREAM("Can't open the serial.");
    return -1;
  }

  if(ser.isOpen())
  {
    ROS_INFO("Succesfully initialize the serial.");
  }
  else
  {
    return -1;
  }

  ros::Subscriber sub = n.subscribe("spray_on", 100, cb);
/*
  while (ros::ok())
  {
    ROS_INFO("Start recevieing joy command.");

    sub = n.subscribe("spray_on", 100, cb);
//    ser.write(cmd);
    cout <<  cmd << endl;
    sleep(1);
    while (ser.available() > 0)
    {
      string msg = ser.read(ser.available());
      ROS_INFO("%s", msg.c_str());
    }
  }
*/
   ros::spin();

  return 0;
}

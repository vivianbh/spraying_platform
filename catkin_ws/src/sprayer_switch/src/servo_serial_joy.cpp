#include <ros/ros.h>
#include <serial/serial.h>
#include <iostream>
#include <string>
#include <sensor_msgs/Joy.h>

using namespace std;

void cb(const sensor_msgs::Joy cmd)
{
  
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "servo_serial");
  ros::NodeHandle n;
  serial::Serial ser;

  ros::Subscriber sub = n.subscribe("sensor_msg/Joy", 100, cb);

  string port;
  int baud;

  if (!n.getParam("baud", baud)) baud = 57600;
  if (!n.getParam("port", port)) port = "/dev/ttyACM0";
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

  while (ros::ok())
  {
    ser.write(cmd);
    sleep(1);
    while (ser.available() > 0)
    {
      string msg = ser.read(ser.available());
      ROS_INFO("%s", msg.c_str());
    }
  }

  return 0;
}

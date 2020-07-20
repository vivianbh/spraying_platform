#include "ros/ros.h"
#include "dynamixel_workbench_msgs/DynamixelCommand.h"
#include <iostream>
#include <cmath>
#include <unistd.h>

using namespace std;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "motion_display2");

  ros::NodeHandle n;
  ros::service::waitForService("/dynamixel_workbench/dynamixel_command");
  ros::ServiceClient client = n.serviceClient<dynamixel_workbench_msgs::DynamixelCommand>("/dynamixel_workbench/dynamixel_command");

  dynamixel_workbench_msgs::DynamixelCommand srv;
  
  int init[3][3] = {{1, 2, 3}, {2170, 2048, 1120}};
  int cmd1[4] = {1658, 2170, 2682, 2170};
  int cmd2[4] = {1536, 2048, 2560, 2048};  

  cout << "Start to initialize" << endl;

  for(int i = 0; i < 3; i++)
  {
    srv.request.id = init[0][i];
    srv.request.addr_name = "Goal_Position";
    srv.request.value = init[1][i];

     if(client.call(srv))
    {
      ROS_INFO_STREAM("Return value = " << (srv.response.comm_result ? "True" : "False"));
    }
    else
    {
      ROS_ERROR("Failed to call service");
      return 1;
    } 
    usleep(1000000);
  }

  cout << "Start moving" << endl;

  for(int i = 0; i < 4; i++)
  {
    srv.request.id = 1;
    srv.request.addr_name = "Goal_Position";
    srv.request.value = cmd1[i];

     if(client.call(srv))
    {
      ROS_INFO_STREAM("Return value = " << (srv.response.comm_result ? "True" : "False"));
    }
    else
    {
      ROS_ERROR("Failed to call service");
      return 1;
    }
    usleep(1000000);
  }

  for(int i = 0; i < 4; i++)
  {
    srv.request.id = 2;
    srv.request.addr_name = "Goal_Position";
    srv.request.value = cmd2[i];

     if(client.call(srv))
    {
      ROS_INFO_STREAM("Return value = " << (srv.response.comm_result ? "True" : "False"));
    }
    else
    {
      ROS_ERROR("Failed to call service");
      return 1;
    }
    usleep(1000000);
  }
  return 0;
}

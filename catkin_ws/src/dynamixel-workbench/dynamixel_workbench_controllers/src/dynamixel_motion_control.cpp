#include "ros/ros.h"
#include "dynamixel_workbench_msgs/DynamixelCommand.h"
#include <iostream>
#include <cmath>

using namespace std;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "motion_client");

  ros::NodeHandle n;
  ros::service::waitForService("/dynamixel_workbench/dynamixel_command");
  ros::ServiceClient client = n.serviceClient<dynamixel_workbench_msgs::DynamixelCommand>("/dynamixel_workbench/dynamixel_command");

  dynamixel_workbench_msgs::DynamixelCommand srv;
  
  int cmd[2];  

  while(1)
  {
    cout << "Please give ID and value" << endl;
    cin >> cmd[0] >> cmd[1];

    srv.request.id = cmd[0];
    srv.request.addr_name = "Goal_Position";
    srv.request.value = cmd[1];

    if(client.call(srv))
    {
      ROS_INFO_STREAM("Return value = " << (srv.response.comm_result ? "True" : "False"));
    }
    else
    {
      ROS_ERROR("Failed to call service");
      return 1;
    }
  }
  return 0;
}

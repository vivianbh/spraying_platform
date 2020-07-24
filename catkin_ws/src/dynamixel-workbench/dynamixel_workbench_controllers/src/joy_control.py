#!/usr/bin/python

import rospy
import subprocess
import math

from sensor_msgs.msg import Joy
from sensor_msgs.msg import JointState
from dynamixel_workbench_msgs.srv import DynamixelCommand

class DriveTeleop:
    def __init__(self):
        self.servo_max = 4095 # max angle of servo rotation
        self.servo_min =  0   # min angle of servo rotation
        self.data_old = 0
        self.servo_center = (self.servo_max + 1) / 2 # center servo position
        self.servo_info = [[1, 2048], [2, 2048], [3, 2048]]
        self.axes0 = 0
        self.axes2 = 0
        self.btn3  = 0
        self.btn1  = 0
        self.btn2  = 0

#        self.joy_sub = rospy.Subscriber("joy", Joy, self.on_joy)

        self.rate = rospy.Rate(30)

        # Initailizing
        for i in range(3):
            resp_result = self.cmd_client("ping", self.servo_info[i][0], "Goal_Position", self.servo_center)
            if resp_result:
                print "Successfully initializing servo id = %d" %(i + 1)
        self.motion()


    def on_joy(self, data):
        if data.axes[0] >= 0.7:
            self.axes0 = 1
        elif data.axes[0] <= -0.7:
            self.axes0 = -1
        else:
            self.axes0 = 0

        if data.axes[2] >= 0.7:
            self.axes2 = 1
        elif data.axes[2] <= -0.7: 
            self.axes2 = -1
        else:
             self.axes2 = 0

        if data.buttons[3] ==1: self.btn3 = 1
        else: self.btn3 = 0

        if data.buttons[1] ==1: self.btn1 = 1
        else: self.btn1 = 0

        if data.buttons[2] ==1: self.btn2 = 1
        else: self.btn2 = 0

    def motion(self):
        print "start motion"
        z = 1
        while z == 1:
            joy_sub = rospy.Subscriber("joy", Joy, self.on_joy)
            # Pan and tilt control (cross)
            if self.axes2 == 1:  # tilt upward (cross up)
                if self.servo_info[1][1] < self.servo_max:
                    self.servo_info[1][1] += 10
            if self.axes2 == -1: # tilt downward (cross down)
                if self.servo_info[1][1] > self.servo_min:
                    self.servo_info[1][1] -= 10
            if self.axes0 == 1 or self.data_old >= 0.7:  # pan leftward (cross left)
                if self.servo_info[0][1] < self.servo_max:
                    self.servo_info[0][1] += 10
                    self.data_old = self.axes0
                if self.axes0 <= 0.7:
                    self.data_old = 0
                self.rate.sleep()
            if self.axes0 == -1 or self.data_old <= -0.7: # pan rightward (cross right)
                if self.servo_info[0][1] < self.servo_max:
                    self.servo_info[0][1] -= 10
                    self.data_old = self.axes0
                if self.axes0 >= -0.7:
                    self.old = 0
                self.rate.sleep()

            # Grippper control
            if self.btn3 == 1: # Open (Y button)
                self.servo_info[2][1] = 1600
                print "gripper open"
            if self.btn1 == 1: # Close (A button)
                self.servo_info[2][1] = 2048
                print "gripper close"

            # Move to home position
            if self.btn2 == 1: # B button
                rospy.loginfo('Move to home position')
                for i in range(3):
                    self.servo_info[i][1] = self.servo_center

            for i in range(3):
                resp_result = self.cmd_client("ping", self.servo_info[i][0], "Goal_Position", self.servo_info[i][1])
                if resp_result:
                    print "Servo position change."
            #self.rate.sleep()

    def cmd_client(self, x, y, z, k):
        rospy.wait_for_service('dynamixel_workbench/dynamixel_command')
        try:
            cmd = rospy.ServiceProxy('dynamixel_workbench/dynamixel_command', DynamixelCommand)
            
            resp = cmd(x, y, z, k)
            return resp.comm_result
        except rospy.ServiceException, e:
            print "Service call faild: %s" %e

#def on_joy(self, data):
#    if data.axes[0] >= 0.7: self.axes0 = 1
#    elif data.axes[0] <= -0.7: self.axes0 = -1
#    else: self.axes0 = 0
#
#    if data.axes[2] >= 0.7: self.axes2 = 1
#    elif data.axes[2] <= -0.7: self.axes2 = -1
#    else: self.axes2 = 0
#
#    if data.buttons[3] ==1: self.btn3 = 1
#    else: self.btn3 = 0
#
#    if data.buttons[1] ==1: self.btn1 = 1
#    else: self.btn1 = 0
#
#    if data.buttons[2] ==1: self.btn2 = 1
#    else: self.btn2 = 0

def main():
    rospy.init_node("joy_control")
    controller = DriveTeleop()

if __name__ =="__main__":
    main()

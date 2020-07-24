#!/usr/bin/python

import rospy
import subprocess
import math

from sensor_msgs.msg import Joy

class DriveTeleop:
    def __init__(self):
        self.servo_max = 4095 # max angle of servo rotation
        self.servo_min =  0   # min angle of servo rotation
        self.servo_center = (self.servo_max + 1) / 2 # center servo position

        self.joy_sub = rospy.Subscriber("joy", Joy, self.on_joy)

    def on_joy(self, data):
        # Pan and tilt control (cross)
        if data.axes[5] == 1:  # tilt upward (cross up)
            rospy.loginfo('Tilt upward')
        if data.axes[5] == -1: # tilt downward (cross down)
            rospy.loginfo('Tilt downward')
        if data.axes[4] == 1:  # pan leftward (cross left)
            rospy.loginfo('Pan leftward')
        if data.axes[4] == -1: # pan rightward (cross right)
            rospy.loginfo('Pan rightward')

        # Grippper control
        if data.buttons[3]: # gripper open (Y button)
            rospy.loginfo('Open')
        if data.buttons[1]: # gripper close (A button)
            rospy.loginfo('Close')

        # Move to home position
        if data.buttons[2]: # B button
            rospy.loginfo('Move to home position')

def main():
    rospy.init_node("joy_control")
    controller = DriveTeleop()
    rospy.spin()

if __name__ =="__main__":
    main()

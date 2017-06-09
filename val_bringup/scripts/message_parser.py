#!/usr/bin/env python

import socket
import time
import subprocess
import rospy
from std_msgs.msg import String
from diagnostic_msgs.msg import DiagnosticArray

FIELD_IP = "192.168.2.10"
#FIELD_IP = "localhost"
PORT = 8080 

class mysocket:
    def __init__(self, sock=None):
        if sock is None:
            self.sock = socket.socket(
                socket.AF_INET, socket.SOCK_STREAM)
        else:
            self.sock = sock

    def connect(self, host, port):
        self.sock.connect((host, port))

    def mysend(self, msg):
        totalsent = 0
        while totalsent < len(msg):
            sent = self.sock.send(msg[totalsent:])
            if sent == 0:
                raise RuntimeError("socket connection broken")
            totalsent = totalsent + sent

    def myreceive(self):
        chunks = []
        bytes_recd = 0
        proceed = True
        while proceed:
            chunk = self.sock.recv(4096)
            if chunk == '':
                raise RuntimeError("socket connection broken")
            if (chunk.endswith("\n")):
                proceed = False
            chunks.append(chunk)
            bytes_recd = bytes_recd + len(chunk)
        return ''.join(chunks)

sock = mysocket()
sock.connect(FIELD_IP,PORT)

def state_callback(data):
    #send only data that is required. it should be a string only
    rospy.loginfo(rospy.get_caller_id() + "Message %s", data.data)
    sock.mysend(data.data +" \n")

def listener():
    # Listen to the decision making topic
    rospy.init_node('state_listener', anonymous=True)
    rospy.Subscriber("/field/log", String, state_callback)



if __name__ == '__main__':
    listener()
    prev_time = time.time()
    prev_ros_time = rospy.get_rostime().secs
    while True:
        current_time = time.time()
        if current_time - prev_time > 5:
            ros_time = rospy.get_rostime().secs
            rtf = (ros_time - prev_ros_time)/ (current_time - prev_time)
            prev_time = time.time()
            prev_ros_time = rospy.get_rostime().secs
            sock.mysend("Real Time Factor : "+ str(rtf))
        msg = sock.myreceive()
        start_index = msg.find("]")
        if start_index == -1:
            continue
        start_index += 2
        if (msg[start_index] == "+"):
            # any message that begins with a + is
            command = msg[start_index+1:-1]
            print msg[:start_index] + "**Command Recieved** " + msg[start_index:-1]
            print "executing '"+command + "'"
            subprocess.Popen(command.split())

        else:
            print msg[:start_index] + "**Message Recieved** " + msg[start_index:-1]

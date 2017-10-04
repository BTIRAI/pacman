from TreeNode import TreeNode
from abc import ABCMeta, abstractmethod
from NodeStatus import NodeStatus


import socket
import sys

HOST = '127.0.0.1'
PORT = 8935
ADDR = (HOST, PORT)

class ControlNode(TreeNode):
    __metaclass__ = ABCMeta  # abstract class

    def __init__(self,name):
        TreeNode.__init__(self,name)
        self.nodeClass = 'Control'
        self.Children = []

        # Create Socket

    def CreateSocket(self):
        try:
            self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except (socket.error):
            print("Failed to create socket")
            sys.exit()
        print("Socket created")

        # Bind to adress
        try:
            self.s.bind((HOST, PORT))
        except (socket.error):
            print("Bind failed. Closing...")
            sys.exit()
        print("Socket bound.")


    def AddChild(self,child):
        self.Children.append(child)

    def GetChildren(self):
        return self.Children

    def GetString(self, string):
        string1 = ""
        for c in self.Children:
            string1 = str(string1 + c.GetString(string))
        return  str(self.GetColor()) + string1


    def HaltChildren(self,h):
        i = 0
        for c in self.Children:
            i = i + 1
            if i >= h:
                if c.GetStatus != NodeStatus.Idle:
                    #print 'Halting Child' + str(c.name)
                    c.Halt()
    def Halt(self):
        self.SetStatus(NodeStatus.Idle)
        self.HaltChildren(1)
from TreeNode import TreeNode
from abc import ABCMeta, abstractmethod
from NodeStatus import NodeStatus

class ControlNode(TreeNode):
    __metaclass__ = ABCMeta  # abstract class

    def __init__(self,name):
        TreeNode.__init__(self,name)
        self.nodeClass = 'Control'
        self.Children = []

    def AddChild(self,child):
        self.Children.append(child)

    def GetChildren(self):
        return self.Children



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
from __future__ import with_statement # 2.5 only
from enum import Enum
from abc import ABCMeta, abstractmethod
from NodeStatus import *
from threading import RLock


NodeState = Enum('NodeState', 'Success Failure Running Idle Halted Exit STATENOTDEFINED')
NodeClass = Enum('NodeClass', 'Control Leaf CLASSNOTDEFINED')
NodeType = Enum('NodeType', 'Sequence Fallback Parallel Decorator Action Condition TYPENOTDEFINED')
class TreeNode:
    __metaclass__ = ABCMeta #abstract class
    def __init__(self,name):

        self.name = name
        self.nodeStatus = NodeStatus.Idle
        self.nodeColor = NodeStatus.Idle
        self.nodeClass = 'Idle'
        self.nodeType = 'Idle'
        self.statusLock = RLock()
        self.colorLock = RLock()


    def GetStatus(self):
        #print 'Getting Status'
        with self.statusLock:
            return self.nodeStatus

    def SetStatus(self,nodeState):
        #print 'Setting Status'
        with self.statusLock:
            self.nodeStatus = nodeState

    def GetColor(self):
        #print 'Getting Status'
        with self.colorLock:
            return self.nodeColor

    def SetColor(self,nodeColor):
        #print 'Setting Status'
        with self.colorLock:
            self.nodeColor = nodeColor

    def PrintName(self):
        print(self.name)

    @abstractmethod
    def Execute(self, args):
        return

    def Halt(self):
        self.SetStatus(NodeStatus.Idle)
        self.SetColor(NodeColor.Black)



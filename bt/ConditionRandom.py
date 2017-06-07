from ConditionNode import ConditionNode
from NodeStatus import *
from random import *
import time
class ConditionRandom(ConditionNode):

    def __init__(self,name):
        ConditionNode.__init__(self,name)


    def Execute(self,args):
        x = randint(1, 10)
        #i = x%2
        if True:
            self.SetStatus(NodeStatus.Failure)
            self.SetColor(NodeColor.Red)
            #print 'checking ' + str(self.name) + ' FAILURE'
        else:
            self.SetStatus(NodeStatus.Success)
            self.SetColor(NodeColor.Green)

from ConditionNode import ConditionNode
from NodeStatus import *
from random import *
import time
class ConditionTest(ConditionNode):

    def __init__(self,name):
        ConditionNode.__init__(self,name)


    def Execute(self):
        x = randint(1, 10)
        i = x%2
        print ('x ' + str(x))
        if i == 0:
            self.SetStatus(NodeStatus.Failure)
            self.SetColor(NodeColor.Red)
            print ('checking ' + str(self.name) + ' FAILURE')
        else:
            self.SetStatus(NodeStatus.Success)
            self.SetColor(NodeColor.Green)

            print ('checking ' + str(self.name) + ' SUCCESS')




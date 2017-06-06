from SequenceNode import *
from SelectorNode import *
from ActionTest import *
from ConditionTest import *
from Draw import *


class BehaviorTree:
        def __init__(self,tree, tick_frequency):
            self.root = tree
            self.tick_frequency = tick_frequency

        def Execute(self):
             while 1:
                    self.root.Execute()
                    time.sleep(1/self.tick_frequency)




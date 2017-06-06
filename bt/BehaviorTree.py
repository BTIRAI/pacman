from SequenceNode import *
from FallbackNode import *
from ActionNode import *
from ConditionNode import *
from Draw import *


class BehaviorTree:
        def __init__(self,tree, tick_frequency):
            self.root = tree
            self.tick_frequency = tick_frequency
            self.current_action = None

        def Execute(self):
             while 1:
                    self.root.Execute()
                    time.sleep(1/self.tick_frequency)




from SequenceNode import *
from FallbackNode import *
from ActionTest import *
from ConditionTest import *
from Draw import *
from BehaviorTree import *

try:
    # for Python2
    import thread
except ImportError:
    # for Python3
    import threading

s1 = FallbackNode('s1')
s2 = SequenceNode('s2')

a1 = ActionTest('Action1')
a2 = ActionTest('Action2')
a3 = ActionTest('Action3')
a4 = ActionTest('Action4')

c1 = ConditionTest('Condition1')
c2 = ConditionTest('c2')

s1.AddChild(c1)
s1.AddChild(a1)


s1.AddChild(a4)

s2.AddChild(c2)
s2.AddChild(s1)

r = BehaviorTree(s2, 10)

try:
    # for Python2
    thread.start_new_thread(r.Execute,())
except NameError:
    # for Python3
    threading._start_new_thread(r.Execute,())

draw_tree(s2)

print('done')


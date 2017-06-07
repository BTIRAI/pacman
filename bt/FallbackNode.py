from ControlNode import ControlNode
from NodeStatus import *
import time

class FallbackNode(ControlNode):

    def __init__(self,name):
        ControlNode.__init__(self,name)
        self.nodeType = 'Selector'


    def Execute(self,args):
        #print 'Starting Children Threads'
        self.SetStatus(NodeStatus.Idle)


        while self.GetStatus() != NodeStatus.Success and self.GetStatus() != NodeStatus.Failure:
            #check if you have to tick a new child or halt the current
            i = 0
            #try:
            for c in self.Children:
                i = i + 1

                if c.GetStatus() == NodeStatus.Idle:
                    #print 'starting tread ' + c.name + ' from thread ' + str(thread.get_ident())

                    #print 'Executing child ' + c.name
                    #thread.start_new_thread(c.Execute,())
                    c.Execute(args)
                   # print '???' + str(i)

                while c.GetStatus() == NodeStatus.Idle:

                    #print 'waiting child ' + c.name + ' thread ' + str(thread.get_ident())
                    time.sleep(0.0001)
                #print 'child ' + c.name + ' status' + str(c.GetStatus())

                if c.GetStatus() == NodeStatus.Running:
                    self.SetStatus(NodeStatus.Running)
                    self.SetColor(NodeColor.Gray)
                  #  print 'Breaking'  + str(i)
                    self.HaltChildren(i + 1)
                    break
                elif c.GetStatus() == NodeStatus.Failure:
                    c.SetStatus(NodeStatus.Idle)


                    if i == len(self.Children):
                        self.SetStatus(NodeStatus.Failure)
                        self.SetColor(NodeColor.Red)
                        break
                        #while self.GetStatus() != NodeStatus.Idle:
                            #time.sleep(0.1)


                elif c.GetStatus() == NodeStatus.Success:
                    c.SetStatus(NodeStatus.Idle)
                    self.HaltChildren(i + 1)
                    self.SetStatus(NodeStatus.Success)
                    self.SetColor(NodeColor.Green)
                    break




                else:
                    raise Exception('Node ' +self.name + ' does not recognize the status of child ' + str(i) +'. (1 is the first)' )



           # except:
             #   print 'Error'



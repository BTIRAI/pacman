from ActionNode import ActionNode
from NodeStatus import *
import time
import random
from distanceCalculator import *


class ActionTest(ActionNode):

    def __init__(self,name):
        ActionNode.__init__(self,name)


    def Execute(self,args):
        self.SetStatus(NodeStatus.Running)
        self.SetColor(NodeColor.Gray)

        while self.GetStatus() == NodeStatus.Running:
            #print self.name + ' executing'
            time.sleep(10)

        self.SetStatus(NodeStatus.Success)



class Greedy(ActionNode):

    def __init__(self,name):
        ActionNode.__init__(self,name)
        self.distances = None

    def Execute(self,args):
        # self.SetStatus(NodeStatus.Running)
        # self.SetColor(NodeColor.Gray)
        self.Directions = args.Directions
        self.distances = args.distances
        print('Executing Action Greedy')
        args.action_executed.SetAction(self.getAction(args.state))
        self.SetStatus(NodeStatus.Success)



    def getAction(self, state):
        # Generate candidate actions
        legal = state.getLegalPacmanActions()
        if self.Directions.STOP in legal: legal.remove(self.Directions.STOP)
        successors = [(state.generateSuccessor(0, action), action) for action in legal]
        scored = [(self.scoreEvaluation(state), action) for state, action in successors]
        bestScore = max(scored)[0]
        bestActions = [pair[1] for pair in scored if pair[0] == bestScore]
        return random.choice(bestActions)

    def scoreEvaluation(self,state):
        return state.getScore()




class Chase(ActionNode):

    def __init__(self,name):
        ActionNode.__init__(self,name)
        self.distances = None

    def Execute(self,args):
        # self.SetStatus(NodeStatus.Running)
        # self.SetColor(NodeColor.Gray)
        self.Directions = args.Directions
        self.distances = args.distances
        print('Executing Action Chase')
        args.action_executed.SetAction(self.getAction(args.state))
        self.SetStatus(NodeStatus.Success)



    def getAction(self, state):
        # Generate candidate actions
        legal = state.getLegalPacmanActions()
        if self.Directions.STOP in legal: legal.remove(self.Directions.STOP)

        successors = [(state.generateSuccessor(0, action), action) for action in legal]
        scored = [(self.closestDistance(state), action) for state, action in successors]
        bestScore = max(scored)[0]
        bestActions = [pair[1] for pair in scored if pair[0] == bestScore]
        return random.choice(bestActions)

    def closestDistance(self,state):

        d1 = getDistanceOnGrid(self.distances, state.getPacmanPosition(), state.getGhostPosition(1))
        d2 = getDistanceOnGrid(self.distances, state.getPacmanPosition(), state.getGhostPosition(2))

        return -min(d1,d2)


class Escape(ActionNode):

    def __init__(self,name):
        ActionNode.__init__(self,name)
        self.distances = None

    def Execute(self,args):
        # self.SetStatus(NodeStatus.Running)
        # self.SetColor(NodeColor.Gray)
        self.Directions = args.Directions
        self.distances = args.distances
        print('Executing Action Escape')
        args.action_executed.SetAction(self.getAction(args.state))
        self.SetStatus(NodeStatus.Success)



    def getAction(self, state):
        # Generate candidate actions
        legal = state.getLegalPacmanActions()
        if self.Directions.STOP in legal: legal.remove(self.Directions.STOP)

        successors = [(state.generateSuccessor(0, action), action) for action in legal]
        scored = [(self.sumDistance(state), action) for state, action in successors]
        bestScore = max(scored)[0]
        bestActions = [pair[1] for pair in scored if pair[0] == bestScore]
        return random.choice(bestActions)

    def sumDistance(self,state):

        if self.distances == None:
           self.distances = computeDistances(state.data.layout)

        d1 = getDistanceOnGrid(self.distances, state.getPacmanPosition(), state.getGhostPosition(1))
        d2 = getDistanceOnGrid(self.distances, state.getPacmanPosition(), state.getGhostPosition(2))
        new_distance = d1 + d2

        return new_distance
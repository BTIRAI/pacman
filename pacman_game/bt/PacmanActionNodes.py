from ActionNode import ActionNode
from NodeStatus import *
import time
import random
from distanceCalculator import *
from searchAgents import *

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
        self.SetColor(NodeColor.Green)




class Greedy(ActionNode):

    def __init__(self,name):
        ActionNode.__init__(self,name)
        self.distances = None

    def Execute(self,args):
        self.SetStatus(NodeStatus.Running)
        self.SetColor(NodeColor.Gray)
        self.Directions = args.Directions
        self.distances = args.distances
        print('Executing Action Greedy')
        args.action_executed.SetAction(self.getAction(args.state))
        self.SetStatus(NodeStatus.Success)
        self.SetColor(NodeColor.Green)




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
        self.distance_calculator = None

    def Execute(self,args):
        if self.distance_calculator is None:
            self.distance_calculator = Distancer(args.state.data.layout)
        self.SetStatus(NodeStatus.Running)
        self.SetColor(NodeColor.Gray)
        self.Directions = args.Directions
        self.distances = args.distances
        print('Executing Action Chase')
        args.action_executed.SetAction(self.getAction(args.state))
        self.SetStatus(NodeStatus.Success)
        self.SetColor(NodeColor.Green)




    def getAction(self, state):
        # Generate candidate actions
        legal = state.getLegalPacmanActions()
        if self.Directions.STOP in legal: legal.remove(self.Directions.STOP)

        successors = [(state.generateSuccessor(0, action), action) for action in legal]
        scored = [(self.closestDistance(state), action) for state, action in successors]
        bestScore = min(scored)[0] #get to the one with the minumim distance
        bestActions = [pair[1] for pair in scored if pair[0] == bestScore]
        actionperformed = random.choice(bestActions)
        # print ('successors', successors)
        # print ('scored', scored)
        #print ('actionperformed',actionperformed)
        #input("Press Enter to continue...")

        return actionperformed

    def closestDistance(self,state):
        minumim_distance = 999 #distance to the closest ghost

        for i in range(state.getNumAgents() - 1):
            ghostState = state.getGhostState(i + 1)
            isGhostScared = ghostState.scaredTimer > 0
            if isGhostScared:
                distance = self.distance_calculator.getDistance(state.getPacmanPosition(), state.getGhostPosition(i+1))
                print('distance from ghost ', i, 'is: ', distance )
                minumim_distance = min (minumim_distance, distance)

        return minumim_distance


class KeepDistance(ActionNode):

    def __init__(self,name):
        ActionNode.__init__(self,name)
        self.distances = None
        self.distance_calculator = None
        self.old_distance = 0


    def Execute(self,args):
        if self.distance_calculator is None:
            self.distance_calculator = Distancer(args.state.data.layout)
        self.SetStatus(NodeStatus.Running)
        self.SetColor(NodeColor.Gray)
        self.Directions = args.Directions
        self.distances = args.distances
        print('Executing Action Keep Distance')
        args.action_executed.SetAction(self.getAction(args.state))
        self.SetStatus(NodeStatus.Success)
        self.SetColor(NodeColor.Green)




    def getAction(self, state):
        # Generate candidate actions
        legal = state.getLegalPacmanActions()
        if self.Directions.STOP in legal: legal.remove(self.Directions.STOP)

        successors = [(state.generateSuccessor(0, action), action) for action in legal]
        scored = [(self.sumDistance(state), action) for state, action in successors]

        bestScore = max(scored)[0]
        bestActions = [pair[1] for pair in scored if pair[0] == bestScore]
        actionperformed = random.choice(bestActions)
        # print ('successors', successors)
        # print ('scored', scored)
        # print ('actionperformed',actionperformed)
        #input("Press Enter to continue...")

        return actionperformed

    def sumDistance(self,state):
        sum_distance = 0
        for i in range(state.getNumAgents() - 1):
            ghostState = state.getGhostState(i + 1)
            isGhostScared = ghostState.scaredTimer > 0
            if not isGhostScared:
                sum_distance += self.distance_calculator.getDistance(state.getPacmanPosition(), state.getGhostPosition(i+1))
        if float(sum_distance).is_integer():
            return sum_distance
            self.old_distance = sum_distance
        else:
            return self.old_distance

class Escape(ActionNode):

    def __init__(self,name):
        ActionNode.__init__(self,name)
        self.distances = None
        self.distance_calculator = None
        self.old_distance = 1000


    def Execute(self,args):
        if self.distance_calculator is None:
            self.distance_calculator = Distancer(args.state.data.layout)
        self.SetStatus(NodeStatus.Running)
        self.SetColor(NodeColor.Gray)
        self.Directions = args.Directions
        self.distances = args.distances
        print('Executing Action Escape')
        args.action_executed.SetAction(self.getAction(args.state))
        self.SetStatus(NodeStatus.Success)
        self.SetColor(NodeColor.Green)




    def getAction(self, state):
        # Generate candidate actions
        legal = state.getLegalPacmanActions()
        if self.Directions.STOP in legal: legal.remove(self.Directions.STOP)



        successors = [(state.generateSuccessor(0, action), action) for action in legal]
        scored = [(self.closestDistance(state), action) for state, action in successors]

        bestScore = max(scored)[0]
        bestActions = [pair[1] for pair in scored if pair[0] == bestScore]
        actionperformed = random.choice(bestActions)
        # print ('successors', successors)
        # print ('scored', scored)
        # print ('actionperformed',actionperformed)
        #input("Press Enter to continue...")

        return actionperformed

    def closestDistance(self,state):
        closest = 1000
        for i in range(state.getNumAgents() - 1):
            ghostState = state.getGhostState(i + 1)
            isGhostScared = ghostState.scaredTimer > 0
            if not isGhostScared:
                closest = min(closest,
                              self.distance_calculator.getDistance(state.getPacmanPosition(), state.getGhostPosition(i+1)))
        if float(closest).is_integer():
            return closest
            self.old_distance = closest
        else:
            return self.old_distance





class ClosestDotSearch(ActionNode):

    def __init__(self,name):
        ActionNode.__init__(self,name)
        self.agent = ClosestDotSearchAgent()
        self.init = False

    def Execute(self,args):
        if not self.init:
            self.agent.registerInitialState(args.state)
            self.init =  True
        self.SetStatus(NodeStatus.Running)
        self.SetColor(NodeColor.Gray)
        self.Directions = args.Directions
        self.distances = args.distances
        print('Executing Action Search')
        legal = args.state.getLegalPacmanActions()
        if self.Directions.STOP in legal: legal.remove(self.Directions.STOP)

        action_executed = self.agent.getAction(args.state)

        if action_executed in legal:
            args.action_executed.SetAction(action_executed)
            self.SetStatus(NodeStatus.Success)
            self.SetColor(NodeColor.Green)
        else:
            #handle illegal move
            self.SetStatus(NodeStatus.Failure)
            self.SetColor(NodeColor.Red)








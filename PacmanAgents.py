# pacmanAgents.py
# ---------------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


from pacman import Directions
from game import Agent, GameStateData
from distanceCalculator import *
import random
import game
import util

import sys
sys.path.insert(0, 'bt/')
#
from SequenceNode import SequenceNode
from FallbackNode import FallbackNode
from PacmanActionNodes import *
from PacmanConditionNodes import *
from ConditionRandom import ConditionRandom

from ActionExecuted import ActionExecuted

#
#

class stru:
    def __init__(self):
        self.state = 0
        self.Directions = 0
        self.action_executed = 0
        self.distances = 0


class BTAgent(Agent):
    def __init__(self):


        fallback_1 = FallbackNode('s1')
        sequence_1 = SequenceNode('s2')

        escape = Escape('Escape')
        greedy = Greedy('Greedy')


        is_close = IsGhostClose('Is Ghost Close', 3)

        sequence_1.AddChild(is_close)
        sequence_1.AddChild(escape)
        fallback_1.AddChild(sequence_1)
        fallback_1.AddChild(greedy)


        self.args = stru()
        self.action_executed = ActionExecuted()
        self.args.distances = None
        self.args.state = None
        self.args.Directions = Directions
        self.args.action_executed = self.action_executed

        self.bt = fallback_1





    def getAction(self, state):
        self.args.state = state
        if self.args.distances == None:
           self.args.distances = computeDistances(state.data.layout)
        self.bt.Execute(self.args)
        return self.action_executed.GetAction()


class GreedyAgent(Agent):
    def __init__(self):
        pass

    def getAction(self, state):
        # Generate candidate actions
        legal = state.getLegalPacmanActions()
        if Directions.STOP in legal: legal.remove(Directions.STOP)

        successors = [(state.generateSuccessor(0, action), action) for action in legal]
        scored = [(self.scoreEvaluation(state), action) for state, action in successors]
        bestScore = max(scored)[0]
        bestActions = [pair[1] for pair in scored if pair[0] == bestScore]
        return random.choice(bestActions)

    def scoreEvaluation(self,state):
        return state.getScore()



class EscapingAgent(Agent):
    def __init__(self):
        self.distances = None

    def getAction(self, state):
        # Generate candidate actions
        legal = state.getLegalPacmanActions()
        if Directions.STOP in legal: legal.remove(Directions.STOP)

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



class ChasingAgent(Agent):
    def __init__(self):
        self.distances = None

    def getAction(self, state):
        # Generate candidate actions
        legal = state.getLegalPacmanActions()
        if Directions.STOP in legal: legal.remove(Directions.STOP)

        successors = [(state.generateSuccessor(0, action), action) for action in legal]
        scored = [(self.closestDistance(state), action) for state, action in successors]
        bestScore = max(scored)[0]
        bestActions = [pair[1] for pair in scored if pair[0] == bestScore]
        return random.choice(bestActions)

    def closestDistance(self,state):

        if self.distances == None:
           self.distances = computeDistances(state.data.layout)

        d1 = getDistanceOnGrid(self.distances, state.getPacmanPosition(), state.getGhostPosition(1))
        d2 = getDistanceOnGrid(self.distances, state.getPacmanPosition(), state.getGhostPosition(2))
        new_distance = d1 + d2

        return -min(d1,d2)



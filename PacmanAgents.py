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
import xml.etree.ElementTree as ET

import sys
sys.path.insert(0, 'bt/')
#
from SequenceNode import SequenceNode
from FallbackNode import FallbackNode
from PacmanActionNodes import *
from PacmanConditionNodes import *
from BehaviorTree import *


from ConditionRandom import ConditionRandom

from ActionExecuted import ActionExecuted
# from Draw import *

#from NewDraw import new_draw_tree
import threading
from time import sleep
import locale
locale.setlocale(locale.LC_NUMERIC, 'C')

class stru:
    def __init__(self):
        self.state = 0
        self.Directions = 0
        self.action_executed = 0
        self.distances = 0


def getBTNode(root):

    type = root.tag

    
    if (type == 'Escape'):
        node = Escape('Escape')
        print('Create Escape')
    elif (type == 'KeepDistance'):
        node = KeepDistance('KeepDistance')
    elif (type == 'Search'):
        node = ClosestDotSearch('Search')
    elif (type == 'Greedy'):
        node = Greedy('Greedy')
    elif (type == 'Chase'):
        node = Chase('Chase')
    elif (type == 'IsGhostClose'):
        print('Create IsGhostClose')
        node = IsGhostClose('IsGhostClose', 5)

    elif (type == 'IsGhostCloser'):
        node = IsGhostClose('IsGhostCloser', 2)
    elif(type == 'IsClosestGhostScared'):
        node = IsClosestGhostScared('IsClosestGhostScared')
    else:
        #is a control node
        if (type == 'Sequence'):
            node = SequenceNode('sequence')
            print('Create Sequence')

        else:
            node = FallbackNode('fallback')
        for child in root:
            node.AddChild(getBTNode(child))

    print('parsing', type)
    return node



class BTAgent(Agent):
    def __init__(self):

        self.args = stru()
        self.action_executed = ActionExecuted()
        self.args.distances = None
        self.args.state = None
        self.args.Directions = Directions
        self.args.action_executed = self.action_executed



        #parsing the tree from the xml file

        xmlTree = ET.parse('pacmantree.xml')
        xmlRoot = xmlTree.getroot()

        root = (xmlRoot[0])[0]

        bt_root = getBTNode(root)


        print(bt_root.GetString(""))
        self.bt = bt_root

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



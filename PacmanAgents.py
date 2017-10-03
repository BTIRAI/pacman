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
import xml.etree.ElementTree as etree

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

    
    if (type is 'Escape'):
        node = Escape('Escape')
    elif (type is 'KeepDistance'):
        node = KeepDistance('KeepDistance')
    elif (type is 'Search'):
        node = ClosestDotSearch('Search')
    elif (type is 'Greedy'):
        node = Greedy('Greedy')
    elif (type is 'Chase'):
        node = Chase('Chase')
    elif (type is 'IsGhostClose'):
        node = IsGhostClose('IsGhostClose', 5)

    elif (type is 'IsGhostCloser'):
        node = IsGhostClose('IsGhostCloser', 2)
    elif(type is 'IsClosestGhostScared'):
        node = IsClosestGhostScared('IsClosestGhostScared')
    else:
        #is a control node
        if (type is 'Sequence'):
            node = SequenceNode('s')
        else:
            node = FallbackNode('f')
        for child in root:
            node.AddChild(getBTNode(child))

    print('parsing', root.tag)
    return node



class BTAgent(Agent):
    def __init__(self):

        fallback_1 = FallbackNode('f1')
        sequence_1 = SequenceNode('s1')
        sequence_2 = SequenceNode('s2')
        sequence_3 = SequenceNode('s3')

        escape = Escape('Escape')
        keep_distance = KeepDistance('KeepDistance')
        search = ClosestDotSearch('Search')
        greedy = Greedy('Greedy')
        chase = Chase('Chase')


        is_close = IsGhostClose('IsGhostClose', 5)
        is_closer = IsGhostClose('IsGhostCloser', 2)
        is_scared = IsClosestGhostScared('IsClosestGhostScared')


        fallback_1.AddChild(is_close)
        fallback_1.AddChild(escape)
        sequence_1.AddChild(fallback_1)
        sequence_1.AddChild(greedy)

        sequence_1.isRoot = True
        sequence_1.CreateSocket()


        #
        # sequence_1.AddChild(is_closer)
        # sequence_1.AddChild(escape)
        #
        # sequence_2.AddChild(is_scared)
        # sequence_2.AddChild(chase)
        #
        # sequence_3.AddChild(is_close)
        # sequence_3.AddChild(escape)
        #
        # fallback_1.AddChild(sequence_1)
        # fallback_1.AddChild(sequence_2)
        # fallback_1.AddChild(sequence_3)
        #
        # fallback_1.AddChild(search)
        # fallback_1.AddChild(greedy)


        self.args = stru()
        self.action_executed = ActionExecuted()
        self.args.distances = None
        self.args.state = None
        self.args.Directions = Directions
        self.args.action_executed = self.action_executed

        #thread.start_new_thread(draw_tree, (fallback_1,))

        #draw_thread = threading.Thread(target=new_draw_tree, args=(fallback_1,))
        #draw_thread.start()



        #parsing the tree from the xml file

        xmlTree = etree.parse('pacmantree.xml')
        xmlRoot = xmlTree.getroot()

        root = (xmlRoot[0])[0]






        self.bt = sequence_1

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



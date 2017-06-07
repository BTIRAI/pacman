from ConditionNode import ConditionNode
from NodeStatus import *
import time
import random
from distanceCalculator import *


class IsGhostClose(ConditionNode):
    def __init__(self, name, distance):
        ConditionNode.__init__(self, name)
        self.distance = distance
        self.distances = None

    def Execute(self, args):

        for i in range(args.state.getNumAgents() - 1):
            dist_to_ghost_i = getDistanceOnGrid(args.distances,
                                                args.state.getPacmanPosition(), args.state.getGhostPosition(i + 1))
            if dist_to_ghost_i <= self.distance:
                self.SetStatus(NodeStatus.Success)
                self.SetColor(NodeColor.Green)
                return

        self.SetStatus(NodeStatus.Failure)
        self.SetColor(NodeColor.Red)


class IsClosestGhostScared(ConditionNode):
    def __init__(self, name, distance):
        ConditionNode.__init__(self, name)
        self.distance = distance
        self.distances = None

    def Execute(self, args):

        for i in range(args.state.getNumAgents() - 1):
            dist_to_ghost_i = getDistanceOnGrid(args.distances,
                                                args.state.getPacmanPosition(), args.state.getGhostPosition(i + 1))
            if dist_to_ghost_i <= self.distance:
                self.SetStatus(NodeStatus.Success)
                self.SetColor(NodeColor.Green)
                return

        self.SetStatus(NodeStatus.Failure)
        self.SetColor(NodeColor.Red)




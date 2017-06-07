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
    def __init__(self, name):
        ConditionNode.__init__(self, name)
        self.distances = None
        self.closest_ghost_dist = 10000
        self.closest_ghost_index = 0

    def Execute(self, args):



        for i in range(args.state.getNumAgents() - 1):
            dist_to_ghost_i = getDistanceOnGrid(args.distances,
                                                args.state.getPacmanPosition(), args.state.getGhostPosition(i + 1))
            if dist_to_ghost_i < self.closest_ghost_dist:
                print('the ghost is closer',)

                self.closest_ghost_dist = dist_to_ghost_i
                self.closest_ghost_index = i + 1

        print ('Closest Ghost is',self.closest_ghost_index )
        ghostState = args.state.getGhostState( self.closest_ghost_index )

        isGhostScared = ghostState.scaredTimer > 0

        if isGhostScared:
            self.SetStatus(NodeStatus.Success)
            self.SetColor(NodeColor.Green)
        else:
            self.SetStatus(NodeStatus.Failure)
            self.SetColor(NodeColor.Red)







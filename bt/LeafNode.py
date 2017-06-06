from abc import ABCMeta, abstractmethod
from TreeNode import TreeNode


class LeafNode(TreeNode):
    __metaclass__ = ABCMeta  # abstract class

    def __init__(self, name):
        TreeNode.__init__(self,name)
        self.nodeClass = 'Leaf'

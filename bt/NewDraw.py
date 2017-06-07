import gi
gi.require_version('Gtk', '3.0')

from gi.repository import Gtk

import xdot
from NodeStatus import *
from threading import RLock
from BehaviorTree import *





class MyDotWindow(xdot.DotWindow):

    def __init__(self):
        xdot.DotWindow.__init__(self)

dotcode = """
digraph G {
    World[label=\"*\n?\" penwidth=\"2\"  shape=\"box\" color=\"black\" ]
    Hello -> World
    Ciao -> World
    Ciao2 -> World
    Ciao3 -> World
}
"""

# std::string DotBt::defineNodeDot(TreeNode* node, const std::string& alias)
# {
#   std::string output = alias + " ";
#
#   // Find the type of the node and its shape and symbol (label).
#   switch (node->DrawType())
#   {
#     case SELECTORSTAR:
#       output += "[label=\"*\n?\" penwidth=\"2\"  shape=\"box\"";
#       break;
#     case BT::SEQUENCESTAR:
#       output += "[label=\"*\n-->\" penwidth=\"2\"  shape=\"box\"";
#       break;
#     case BT::SELECTOR:
#       output += "[label=\"?\" penwidth=\"2\"  shape=\"box\"";
#       break;
#     case BT::SEQUENCE:
#       output += "[label=\"-->\" penwidth=\"2\"  shape=\"box\"";
#       break;
#     case BT::PARALLEL:
#       output += "[label=\"-->\n-->\" penwidth=\"2\"  shape=\"box\"";
#       break;
#     case BT::DECORATOR:
#       output += "[label=\"D\" penwidth=\"2\" shape=\"diamond\"";
#       break;
#     case BT::ACTION:
#       output += "[label=\"" + node->get_name() + "\" penwidth=\"2\" shape=\"box\" fillcolor=\"palegreen\" style=\"filled\"";
#       break;
#     case BT::CONDITION:
#       output += "[label=\"" + node->get_name() + "\" penwidth=\"2\" shape=\"ellipse\" fillcolor=\"khaki1\" style=\"filled\"";
#       break;
#     default:
#       break;
#   }
#
#   // Get the current status of the node for the coloring.
#   switch (node->get_color_status())
#   {
#     case BT::RUNNING:
#       output += " color=\"black\" ];";
#       break;
#     case BT::SUCCESS:
#       output += " color=\"green\" ];";
#       break;
#     case BT::FAILURE:
#       output += " color=\"red\" ];";
#       break;
#     case BT::IDLE:
#       output += " color=\"gray88\" ];";
#       break;
#     case BT::HALTED:
#       output += " color=\"orange\" ];";
#       break;
#     default:
#       output += " color=\"gray88\" ];";
#       break;
#   }
#
#   return output;
# }



def main():
    window = MyDotWindow()
    window.set_dotcode(dotcode)
    window.connect('delete-event', Gtk.main_quit)
    Gtk.main()


if __name__ == '__main__':
    main()


# def updateDraw(canvas,tree,xPos, yPos,xPosParent,yPosParent,depth):
#     #print 'class '+ tree.nodeClass
#     #print 'type '+ tree.nodeType
#
#
#     if tree.nodeClass == 'Leaf':
#         drawNode(canvas,tree,xPos,yPos,xPosParent,yPosParent)
#     else:
#         drawNode(canvas,tree,xPos,yPos,xPosParent,yPosParent)
#         yPos_ = yPos + 50
#         xPos_ = xPos
#         childrenWidth = getDistance()*depth
#         distance = 30
#         nOfChild = len(tree.Children)
#         for c in tree.Children:
#             childrenWidth = childrenWidth + max(8*len(c.name)+4,50) + distance
#             offset = 0
#             if len(tree.Children)%2 == 0:
#                 offset = 4*len(tree.Children) + 4
#             else:
#                 offset = 4*(len(tree.Children)-1) + 4
#             xPos_ = xPos - childrenWidth/2 -distance - nOfChild*offset - getDistance()*depth
#         for c in tree.Children:
#             xPos_ = xPos_ + max(8*len(c.name)+4,50) + distance + getDistance()*depth + offset
#             updateDraw(canvas,c,xPos_,yPos_,xPos,yPos,depth/2)
#
# def init(canvas,tree,root):
#     redrawAll(canvas,tree,root)
#     #updateDraw(canvas,tree,400,10, -1,-1)#400 is in the middle of the canvas (800x600)
#
# def drawNode(canvas, tree,xPos,yPos,xPosParent,yPosParent):
#
#     if tree.GetColor() == NodeColor.Gray:
#         nodeColor = 'gray'
#     elif tree.GetColor() == NodeColor.Green:
#         nodeColor = 'green'
#     elif tree.GetColor() == NodeColor.Red:
#         nodeColor = 'red'
#     else:
#         nodeColor = 'black'
#
#
#     if tree.nodeType == 'Sequence':
#         #print 'Drawing a sequence'
#         xSize = 15
#         ySize = 15
#         canvas.create_rectangle(0+xPos-xSize/2,  0+yPos-ySize/2, 15+xPos-xSize/2, 15+yPos-ySize/2, outline=nodeColor, stipple="")
#         canvas.create_text(xPos,yPos,fill="black",font="Times 15 bold",
#                         text=u'\u2192')
#         if xPosParent > 0:#avoid drawing the first line
#             canvas.create_line(xPosParent,yPosParent+7.5, xPos, yPos -ySize/2)
#
#     if tree.nodeType == 'Selector':
#         #print 'Drawing a selector'
#         xSize = 15
#         ySize = 15
#         canvas.create_rectangle(0+xPos-xSize/2,  0+yPos-ySize/2, 15+xPos-xSize/2, 15+yPos-ySize/2, outline=nodeColor, stipple="")
#         canvas.create_text(xPos,yPos,fill="black",font="Times 15 bold",
#                         text='?')
#         if xPosParent > 0:#avoid drawing the first line
#             canvas.create_line(xPosParent,yPosParent+7.5, xPos, yPos -ySize/2)
#
#
#     if tree.nodeType == 'Action':
#         #print 'Drawing an action'
#         xSize = max(8*len(tree.name)+4,50)
#         ySize = 20
#         canvas.create_rectangle(xPos-xSize/2,  yPos-ySize/2, xSize+xPos-xSize/2, ySize+yPos - ySize/2, outline=nodeColor,fill='green', stipple="")
#         canvas.create_text(xPos,yPos,fill="black",font="Times 15 bold",
#                         text=tree.name )
#         canvas.create_line(xPosParent,yPosParent + 7.5, xPos, yPos - ySize/2)
#
#     if tree.nodeType == 'Condition':
#         #print 'Drawing a Condition'
#         xSize = max(8*len(tree.name)+4,50)
#         ySize = 20
#         #canvas.create_rectangle(xPos,  yPos, xSize+xPos, ySize+yPos, outline="black",fill='green', stipple="")
#
#         #canvas.create_oval(xPos, yPos, xSize+xPos, ySize+yPos, fill="yellow", stipple="")
#         canvas.create_oval(xPos-xSize/2,  yPos-ySize/2, xSize+xPos-xSize/2, ySize+yPos - ySize/2, outline=nodeColor,fill='yellow', stipple="")
#         canvas.create_text(xPos,yPos,fill="black",font="Times 15 bold",
#                         text=tree.name)
#         canvas.create_line(xPosParent,yPosParent + 7.5, xPos, yPos - ySize/2)
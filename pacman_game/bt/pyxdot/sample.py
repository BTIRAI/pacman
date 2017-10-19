#!/usr/bin/env python3
#
# Copyright 2008 Jose Fonseca
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#


import gi
gi.require_version('Gtk', '3.0')

from gi.repository import Gtk

import xdot


class MyDotWindow(xdot.DotWindow):

    def __init__(self):
        xdot.DotWindow.__init__(self)
        self.dotwidget.connect('clicked', self.on_url_clicked)

    def on_url_clicked(self, widget, url, event):
        dialog = Gtk.MessageDialog(
            parent=self,
            buttons=Gtk.ButtonsType.OK,
            message_format="%s clicked" % url)
        dialog.connect('response', lambda dialog, response: dialog.destroy())
        dialog.run()
        return True


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

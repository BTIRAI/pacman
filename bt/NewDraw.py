import gi
gi.require_version('Gtk', '3.0')

from gi.repository import Gtk, GdkPixbuf, Gdk, GLib

import xdot
from NodeStatus import *
from threading import RLock
from BehaviorTree import *
from gi.repository import GObject

import random
from time import sleep


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


class BTWindow(xdot.DotWindow):
    def __init__(self):
        xdot.DotWindow.__init__(self)
        self.dotcode = None


    def get_dot_code(self):

        return self.dotcode

    def update_widget(self):
        print('update')

        print('getting code')
        symbols = ['black', 'red', 'yellow']
        random.seed
        symbol = random.choice(symbols)
        self.dotcode = """
        digraph G {
            World[label=
            """ + symbol + """
            penwidth=\"2\"  shape=\"box\" color=\"black\" ]
            Hello -> World
            Ciao -> World
            Ciao2 -> World
            Ciao3 -> World
        }
        """


        GLib.timeout_add_seconds(1, self.update_widget)
        self.set_dotcode(self.dotcode)
        pass



def draw():
    window = BTWindow()
    window.update_widget()
    # dcode = window.get_dot_code()
    # window.set_dotcode(dcode)
    window.connect('delete-event', Gtk.main_quit)

    #Gtk.timeout_add(250, update_widget)  # call update_widget every 250 ms
    #Gtk.main_iteration_do(True)

    Gtk.main()



def main():
    window = MyDotWindow()
    window.set_dotcode(dotcode)
    window.connect('delete-event', Gtk.main_quit)
    Gtk.main()


if __name__ == '__main__':
    # main()
    draw()




# from gi.repository import Gtk, GdkPixbuf, Gdk, GLib
# import os, sys
# import time
#
# class GUI:
#     def __init__(self):
#         self.window=Gtk.Window()
#         self.window.set_title("Countdown")
#         self.window.set_border_width(10)
#         self.window.connect_after('destroy', self.destroy)
#
#         # main container of projct
#         self.main_box=Gtk.VBox()
#         self.main_box.set_spacing(5)
#
#         # countdown label
#         self.countdown_label = Gtk.Label()
#
#         # start button
#         self.start_button=Gtk.Button("Start!")
#
#         # add the elements to the window
#         self.window.add(self.main_box)
#         self.main_box.pack_start(self.countdown_label, False, False, 0)
#         self.main_box.pack_start(self.start_button, False, False, 0)
#
#         # connect buttons
#         self.start_button.connect_after('clicked', self.start_clicked)
#
#         self.window.show_all()
#
#     def start_clicked(self, button):
#         self.countdown(3)
#
#     def countdown(self, count):
#         if count > 0:
#             self.countdown_label.set_text(str(count))
#             # Manually re-add this callback with a decremented count.
#             GLib.timeout_add_seconds(2, self.countdown, count - 1)
#         else:
#             self.countdown_label.set_text("Go!")
#
#         # Return False so the callback is not called repeatedly
#         # as we manually re-added the callback ourselves above.
#         return False
#
#     def destroy(window, self):
#         Gtk.main_quit()
#
# def main():
#     app=GUI()
#     Gtk.main()
#
# if __name__ == "__main__":
#     sys.exit(main())

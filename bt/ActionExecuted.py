from threading import RLock

class ActionExecuted:
        def __init__(self):
            self.action = None
            self.timeLock = RLock()


        def GetAction(self):
            return self.action

        def SetAction(self,action):
            self.action = action
try:
    # for Python2
    from Tkinter import  *
except:
    # for Python3
    from tkinter import  *



from NodeStatus import *
from threading import RLock


positionLock = RLock()
offsetX = 0.0
offsetY = 0.0
distance = 0.0


def getDistance():
    global positionLock
    global distance
    with positionLock:
        return max(distance,0)

def setDistance(newDistance):
    global positionLock
    global distance
    with positionLock:
            distance = newDistance

def getOffsetX():
    global positionLock
    global offsetX
    with positionLock:
        return offsetX

def getOffsetY():
    global positionLock
    global  offsetY
    with positionLock:
        return offsetY

def setOffsetX(newOffsetX):
    global positionLock
    global offsetX
    with positionLock:
            offsetX = newOffsetX

def setOffsetY(newOffsetY):
    global positionLock
    global offsetY
    with positionLock:
            offsetY = newOffsetY

def redrawAll(canvas,tree,root):
    canvas.delete(ALL)

    updateDraw(canvas,tree,root.winfo_width()/2 + getOffsetX(),10 + getOffsetY(),  -1,-1,1.0)#400 is in the middle of the canvas (800x600)
    root.after(10,redrawAll,canvas,tree,root)

def updateDraw(canvas,tree,xPos, yPos,xPosParent,yPosParent,depth):
    #print 'class '+ tree.nodeClass
    #print 'type '+ tree.nodeType


    if tree.nodeClass == 'Leaf':
        drawNode(canvas,tree,xPos,yPos,xPosParent,yPosParent)
    else:
        drawNode(canvas,tree,xPos,yPos,xPosParent,yPosParent)
        yPos_ = yPos + 50
        xPos_ = xPos
        childrenWidth = getDistance()*depth
        distance = 30
        nOfChild = len(tree.Children)
        for c in tree.Children:
            childrenWidth = childrenWidth + max(8*len(c.name)+4,50) + distance
            offset = 0
            if len(tree.Children)%2 == 0:
                offset = 4*len(tree.Children) + 4
            else:
                offset = 4*(len(tree.Children)-1) + 4
            xPos_ = xPos - childrenWidth/2 -distance - nOfChild*offset - getDistance()*depth
        for c in tree.Children:
            xPos_ = xPos_ + max(8*len(c.name)+4,50) + distance + getDistance()*depth + offset
            updateDraw(canvas,c,xPos_,yPos_,xPos,yPos,depth/2)

def init(canvas,tree,root):
    redrawAll(canvas,tree,root)
    #updateDraw(canvas,tree,400,10, -1,-1)#400 is in the middle of the canvas (800x600)

def drawNode(canvas, tree,xPos,yPos,xPosParent,yPosParent):

    if tree.GetColor() == NodeColor.Gray:
        nodeColor = 'gray'
    elif tree.GetColor() == NodeColor.Green:
        nodeColor = 'green'
    elif tree.GetColor() == NodeColor.Red:
        nodeColor = 'red'
    else:
        nodeColor = 'black'


    if tree.nodeType == 'Sequence':
        #print 'Drawing a sequence'
        xSize = 15
        ySize = 15
        canvas.create_rectangle(0+xPos-xSize/2,  0+yPos-ySize/2, 15+xPos-xSize/2, 15+yPos-ySize/2, outline=nodeColor, stipple="")
        canvas.create_text(xPos,yPos,fill="black",font="Times 15 bold",
                        text=u'\u2192')
        if xPosParent > 0:#avoid drawing the first line
            canvas.create_line(xPosParent,yPosParent+7.5, xPos, yPos -ySize/2)

    if tree.nodeType == 'Selector':
        #print 'Drawing a selector'
        xSize = 15
        ySize = 15
        canvas.create_rectangle(0+xPos-xSize/2,  0+yPos-ySize/2, 15+xPos-xSize/2, 15+yPos-ySize/2, outline=nodeColor, stipple="")
        canvas.create_text(xPos,yPos,fill="black",font="Times 15 bold",
                        text='?')
        if xPosParent > 0:#avoid drawing the first line
            canvas.create_line(xPosParent,yPosParent+7.5, xPos, yPos -ySize/2)


    if tree.nodeType == 'Action':
        #print 'Drawing an action'
        xSize = max(8*len(tree.name)+4,50)
        ySize = 20
        canvas.create_rectangle(xPos-xSize/2,  yPos-ySize/2, xSize+xPos-xSize/2, ySize+yPos - ySize/2, outline=nodeColor,fill='green', stipple="")
        canvas.create_text(xPos,yPos,fill="black",font="Times 15 bold",
                        text=tree.name )
        canvas.create_line(xPosParent,yPosParent + 7.5, xPos, yPos - ySize/2)

    if tree.nodeType == 'Condition':
        #print 'Drawing a Condition'
        xSize = max(8*len(tree.name)+4,50)
        ySize = 20
        #canvas.create_rectangle(xPos,  yPos, xSize+xPos, ySize+yPos, outline="black",fill='green', stipple="")

        #canvas.create_oval(xPos, yPos, xSize+xPos, ySize+yPos, fill="yellow", stipple="")
        canvas.create_oval(xPos-xSize/2,  yPos-ySize/2, xSize+xPos-xSize/2, ySize+yPos - ySize/2, outline=nodeColor,fill='yellow', stipple="")
        canvas.create_text(xPos,yPos,fill="black",font="Times 15 bold",
                        text=tree.name)
        canvas.create_line(xPosParent,yPosParent + 7.5, xPos, yPos - ySize/2)

def keyCallback(event):
    strPressed = str(repr(event.char))



    if strPressed == '\xef\x9c\x81':
        setOffsetY(getOffsetY() - 10 )


def keyCallbackUp(event):
    setOffsetY(getOffsetY() - 10 )


def keyCallbackDown(event):
    setOffsetY(getOffsetY() + 10 )


def keyCallbackLeft(event):
    setOffsetX(getOffsetX() - 10 )


def keyCallbackRight(event):
    setOffsetX(getOffsetX() + 10 )

def keyCallbackReturn(event):
    setOffsetX(0)
    setOffsetY(0)



def keyCallbackShiftUp(event):
    setOffsetY(getOffsetY() - 100 )

def keyCallbackShiftDown(event):
    setOffsetY(getOffsetY() + 100 )


def keyCallbackShiftLeft(event):
    setOffsetX(getOffsetX() - 100 )


def keyCallbackShiftRight(event):
    setOffsetX(getOffsetX() + 100 )



def keyCallbackPrior(event):
    setDistance(getDistance() + 50 )


def keyCallbackNext(event):
    setDistance(getDistance() - 50 )




def draw_tree(tree):
    # create the root and the canvas
    root = Tk()
    root.title('Behavior Tree')
    #canvas = ResizingCanvas(root, width=root.winfo_screenwidth(), height=root.winfo_screenheight())
    canvas = ResizingCanvas(root,width=root.winfo_screenwidth(), height=root.winfo_screenheight()/2, highlightthickness=0)

    canvas.pack()
    # Store canvas in root and in canvas itself for callbacks
    root.canvas = canvas.canvas = canvas
    # Set up canvas data and call init
    canvas.data = { }
    init(canvas,tree,root)

    # set up events
    # root.bind("<Button-1>", mousePressed)
    # root.bind("<Key>", keyPressed)
    # timerFired(canvas)
    # and launch the app
    frame = Frame(root, width=root.winfo_screenwidth(), height=root.winfo_screenheight())


    #canvas.pack(fill=BOTH, expand=YES)
    canvas.bind("<1>", lambda event: canvas.focus_set())
    canvas.bind("<Up>", keyCallbackUp)
    canvas.bind("<Down>", keyCallbackDown)
    canvas.bind("<Left>", keyCallbackLeft)
    canvas.bind("<Right>", keyCallbackRight)
    canvas.bind("<Return>", keyCallbackReturn)
    canvas.bind("<Return>", keyCallbackReturn)

    canvas.bind("<Shift-Up>", keyCallbackShiftUp)
    canvas.bind("<Shift-Down>", keyCallbackShiftDown)
    canvas.bind("<Shift-Left>", keyCallbackShiftLeft)
    canvas.bind("<Shift-Right>", keyCallbackShiftRight)

    canvas.bind("<Prior>", keyCallbackPrior)
    canvas.bind("<Next>", keyCallbackNext)





    root.after(10,redrawAll,canvas,tree,root)
    root.mainloop()  # This call BLOCKS (so your program waits until you close the window!)

class ResizingCanvas(Canvas):
    def __init__(self,parent,**kwargs):
        Canvas.__init__(self,parent,**kwargs)
        self.bind("<Configure>", self.on_resize)
        self.height = self.winfo_reqheight()
        self.width = self.winfo_reqwidth()

    def on_resize(self,event):
        # determine the ratio of old width/height to new width/height
        wscale = float(event.width)/self.width
        hscale = float(event.height)/self.height
        self.width = event.width
        self.height = event.height
        # resize the canvas
        self.config(width=self.width, height=self.height)
        # rescale all the objects tagged with the "all" tag
        self.scale("all",0,0,wscale,hscale)

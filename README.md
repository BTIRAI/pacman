Installation in Windows
------------
1) Install Dependencies:

Download and install [Cmake for Windows](https://cmake.org/download/) <br/>
Download and install [QT5 for Windows](https://www.qt.io/download-qt-for-application-development) (the open source version is free)  <br/>
Download and install [python3 for Windows](http://docs.python-guide.org/en/latest/starting/install3/win/)<br/>
2) Download the repository:

`git clone https://github.com/btirai/pacmanBT.git`

3) Build the Software:

Run Cmake, set the project (pacmanBT) root folder and the desired build folder (your choice). Configure and generate project solution for your favorite IDE (e.g. Visual Studio 13). Then open the solution from your IDE and build the project.


Run in Windows
------------

In a terminal, go to the build directory of the project and then run the following commands:

`cd bt_editor`

`./behavior_tree_editor.exe`

Alternatively, run the executable behavior_tree_editor.exe from File Explorer



Installation in Unix
------------
1) Install Dependencies:

`sudo apt-get install build-essential libgl1-mesa-dev python3-tk qtdeclarative5-dev`

2) Download the repository:

`git clone https://github.com/miccol/pacmanBT.git`

3) Build the Software:

Run in a terminal the following commands:

`cd pacmanBT` <br/>
`mkdir build` <br/>
`cd build` <br/>
`cmake ..` <br/>
`make`

Run in Unix
------------

Run in a terminal the following commands:

`cd bt_editor`

`./behavior_tree_editor`







Verify your installation
------------
-In the BT editor: File->Load  <br/>
-Browse in the folder pacmanBT (the one you retrieved in step 2)  <br/>
-Select the file pacmantree.xml  <br/>
-A BT should appear in the editor, as in the picture below: <br/>

![alt tag](https://github.com/miccol/pacmanBT/blob/master/ScreenWindows.jpg)





-Press the Play <img src="https://github.com/miccol/pacmanBT/blob/master/bt_editor/qt_nodeditor/resources/play.png" width="15" height="15"> icon. <br/>
-Enjoy


Create your Behavior Tree
------------

Right Click in the editor to add a node. 
Press Play to run the BT
Enjoy

NOTE: A BT must have the root node. It cannot have loose nodes (non-connected nodes).











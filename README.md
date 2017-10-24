DEPENDENCIES
------------


TODO


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
-A BT should appear in the editor <br/>
-Press the Play icon. <br/>
-Enjoy


Create your Behavior Tree
------------

Right Click in the editor to add a node. 
Press Play to run the BT
Enjoy

NOTE: A BT must have the root node. It cannot have loose nodes (non-connected nodes).











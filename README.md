DEPENDENCIES
------------


* [YARP](https://www.yarp.it/)
* [LUA](https://www.lua.org/)


Instruction in Unix
------------
1) Install Dependencies:

`sudo apt-get install build-essential libgl1-mesa-dev python3-tk`

2) Download the repository

`git clone https://github.com/miccol/pacmanBT.git`

3) Build the Software

`cd pacmanBT`

`mkdir build`

`cd build`

`cmake ..`

`make`


4) Run the Software

`cd bt_editor`

`./behavior_tree_editor`

5)Run a sample tree 
In the BT editor: File->Load
Browse in the folder pacmanBT (the one you retrieved in step 2)
select the file pacmantree.xml
A BT should appear in the editor
Press the Play icon.
Enjoy








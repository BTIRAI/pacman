DEPENDENCIES
------------


* [YARP](https://www.yarp.it/)
* [LUA](https://www.lua.org/)


Instruction in Unix
------------
1) Install Dependencies:

`sudo apt-get install build-essential libgl1-mesa-dev python3-tk`

2) Download the repository:

`git clone https://github.com/miccol/pacmanBT.git`

3) Build the Software:

Run in a terminal the following commands:

`cd pacmanBT` <br/>
`mkdir build` <br/>
`cd build` <br/>
`cmake ..` <br/>
`make`


4) Run the Software:

Run in a terminal the following commands:

`cd bt_editor`

`./behavior_tree_editor`

5)Run a sample tree:

-In the BT editor: File->Load  <br/>
-Browse in the folder pacmanBT (the one you retrieved in step 2)  <br/>
-Select the file pacmantree.xml  <br/>
-A BT should appear in the editor <br/>
-Press the Play icon. <br/>
-Enjoy








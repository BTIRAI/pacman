Installation in Windows
------------
1. Install Dependencies:

    Download and install [Cmake for Windows](https://cmake.org/download/) <br/>
    Download and install [QT5 for Windows](https://www.qt.io/download-qt-for-application-development) (the open source version is free)  <br/>
    Download and install [python3 for Windows](http://docs.python-guide.org/en/latest/starting/install3/win/)<br/>

1. Download the repository:

    ```shell
    git clone https://github.com/btirai/pacman.git`
    ```

1. Build the Software:

    Run Cmake, set the project (`pacman`) root folder and the desired build folder (your choice). Configure and generate project solution for your favorite IDE (e.g. Visual Studio 13). Then open the solution from your IDE and build the project.


Run in Windows
------------

In a terminal, go to the build directory of the project and then run the following commands:

```shell
cd bt_editor
./behavior_tree_editor.exe
```

Alternatively, run the executable `behavior_tree_editor.exe` from File Explorer

Installation in Unix
------------
1. Install Dependencies:

    ```shell
    sudo apt-get install build-essential libgl1-mesa-dev python3-tk qtdeclarative5-dev
    ```

1. Download the repository:

    ```shell
    git clone https://github.com/btirai/pacman.git
    ```

1. Build the Software:

    Run in a terminal the following commands:

    ```shell
    cd pacman
    mkdir build
    cd build
    cmake ..
    make
    ```

Run in Unix
------------

From the build directory, run in a terminal with

`./behavior_tree_editor`

Verify your installation
------------
- In the BT editor: `File`->`Load`  <br/>
- Browse in the folder pacman (the one you retrieved in step 2)  <br/>
- Select the file `pacmantree.xml`  <br/>
- A BT should appear in the editor, as in the picture below: <br/>

    ![screenshot](./ScreenWindows.jpg)

- Press the Play <img src="bt_editor/qt_nodeditor/resources/play.png" width="15" height="15"> icon. <br/>
- Enjoy


Create your Behavior Tree
------------

1. Right Click in the editor to add a node.
1. Press Play to run the BT
1. Enjoy

NOTE: A BT must have the root node. It cannot have loose nodes (non-connected nodes).

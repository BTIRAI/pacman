cmake_minimum_required(VERSION 3.2)

project(behavior_tree_editor)
add_definitions(-std=c++11  -pthread)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")





if(APPLE AND EXISTS /usr/local/opt/qt5)
	# Homebrew installs Qt5 (up to at least 5.9.1) in
	# /usr/local/qt5, ensure it can be found by CMake since
	# it is not in the default /usr/local prefix.
	list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt5")
endif()


find_package(Qt5 COMPONENTS  Core Widgets Gui OpenGL Xml)

find_package( Threads )



#############################################################
# http://answers.ros.org/question/230877/optionally-build-a-package-with-catkin/
if( CATKIN_DEVEL_PREFIX OR CATKIN_BUILD_BINARY_PACKAGE)
    set(catkin_FOUND 1)
    add_definitions( -DUSING_ROS )
endif()

if(catkin_FOUND)
  find_package(catkin REQUIRED COMPONENTS behavior_tree_cpp rosbag rosbag_storage roscpp)

  message(STATUS "------------------------------------------------")
  message(STATUS "BehaviourTreeEditor is being built using CATKIN.")
  message(STATUS "------------------------------------------------")

  catkin_package(
    INCLUDE_DIRS 
    LIBRARIES qt_node_editor
    CATKIN_DEPENDS behavior_tree_cpp
    DEPENDS 
    )
endif(catkin_FOUND)
#############################################################

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_definitions(${Qt5Widgets_DEFINITIONS} -DNODE_EDITOR_STATIC -std=c++11)
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")


INCLUDE_DIRECTORIES( ${catkin_INCLUDE_DIRS} ./qt_nodeditor/include )



#########################################################
# FIND Lua
#########################################################
find_package(Lua REQUIRED)
MESSAGE(STATUS "LUA_DIR:" ${LUA_LIBRARIES})
INCLUDE_DIRECTORIES(${LUA_INCLUDE_DIR})




#INCLUDE_DIRECTORIES(${LUA_DIR}/include/)

file(GLOB_RECURSE LIB_CPPS  ./qt_nodeditor/src/*.cpp )
file(GLOB_RECURSE APP_CPPS  ./bt_editor/*.cpp )

qt5_add_resources(RESOURCES ./qt_nodeditor/resources/resources.qrc)

# Tell CMake to create the helloworld executable
add_library(qt_node_editor STATIC ${LIB_CPPS} ${RESOURCES})

target_link_libraries(qt_node_editor
                      Qt5::Core  Qt5::Widgets
                      Qt5::Gui   Qt5::OpenGL
                      Qt5::Xml)

QT5_WRAP_UI(FORMS_HEADERS
  ./bt_editor/mainwindow.ui
  ./bt_editor/action_form.ui
  ./bt_editor/condition_form.ui)

message("the value of BTSrcLibrary  EDITOR: " ${BTSrcLibrary})
message("the value of BTSrcLibrary  EDITOR: " ${BTHeadLibrary})


add_executable(behavior_tree_editor ${APP_CPPS} ${FORMS_HEADERS})
target_link_libraries(behavior_tree_editor qt_node_editor
    ${catkin_LIBRARIES} ${LUA_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )

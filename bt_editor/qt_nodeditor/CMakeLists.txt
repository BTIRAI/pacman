cmake_minimum_required(VERSION 2.8)

project(behavior_tree_editor)

find_package(Qt5 COMPONENTS  Core Widgets Gui OpenGL Xml)

find_package(catkin REQUIRED COMPONENTS
  rosbag
  rosbag_storage
  roscpp
  rostime
  )

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES qt_node_editor
  CATKIN_DEPENDS
  DEPENDS
  )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_definitions(${Qt5Widgets_DEFINITIONS} -DNODE_EDITOR_STATIC -std=c++11)
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)


INCLUDE_DIRECTORIES( ${catkin_INCLUDE_DIRS} include )

#############################################################

file(GLOB_RECURSE LIB_CPPS  ./src/*.cpp )
file(GLOB_RECURSE APP_CPPS  ./behavior-tree/*.cpp )

qt5_add_resources(RESOURCES ./resources/resources.qrc)

# Tell CMake to create the helloworld executable
add_library(qt_node_editor STATIC ${LIB_CPPS} ${RESOURCES})

target_link_libraries(qt_node_editor
                      Qt5::Core  Qt5::Widgets
                      Qt5::Gui   Qt5::OpenGL
                      Qt5::Xml)

QT5_WRAP_UI(FORMS_HEADERS
  ./behavior-tree/mainwindow.ui
  ./behavior-tree/action_form.ui )

add_executable(behavior-tree ${APP_CPPS} ${FORMS_HEADERS} )
target_link_libraries(behavior-tree
    qt_node_editor
    ${catkin_LIBRARIES}
    Qt5::Core  Qt5::Widgets
    Qt5::Gui   Qt5::OpenGL
    Qt5::Xml)

cmake_minimum_required(VERSION 2.8)

project(behavior_tree_editor)
add_definitions(-std=c++11  -pthread)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")


#set(Python_ADDITIONAL_VERSIONS 3.6)



if(APPLE AND EXISTS /usr/local/opt/qt5)
	# Homebrew installs Qt5 (up to at least 5.9.1) in
	# /usr/local/qt5, ensure it can be found by CMake since
	# it is not in the default /usr/local prefix.
	list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt5")
endif()


find_package(Qt5 COMPONENTS  Core Widgets Gui OpenGL Xml)

find_package( Threads )



set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_definitions(${Qt5Widgets_DEFINITIONS} -DNODE_EDITOR_STATIC -std=c++11)
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")


INCLUDE_DIRECTORIES( ${catkin_INCLUDE_DIRS} ./qt_nodeditor/include )


INCLUDE_DIRECTORIES( C:/Users/mcolledanchise/AppData/Local/Programs/Python/Python36-32/include )



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



add_executable(behavior_tree_editor ${APP_CPPS} ${FORMS_HEADERS} )
#target_link_libraries(behavior_tree_editor qt_node_editor
#    ${catkin_LIBRARIES} ${LUA_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} -lws2_32)

target_link_libraries(behavior_tree_editor qt_node_editor
    ${catkin_LIBRARIES} ${LUA_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

add_custom_command(TARGET behavior_tree_editor PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/pacman_game $<TARGET_FILE_DIR:behavior_tree_editor>)


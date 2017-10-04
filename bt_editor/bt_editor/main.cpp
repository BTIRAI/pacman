
#include <QApplication>
#include <node_editor/NodeStyle>
#include <node_editor/FlowViewStyle>
#include <node_editor/ConnectionStyle>

#include "ControlNodeModel.hpp"
#include "mainwindow.h"
#include "ActionNodeModel.hpp"
#include "RootNodeModel.hpp"
#include "NodeFactory.hpp"

#include <node_editor/DataModelRegistry>

using QtNodes::DataModelRegistry;
using QtNodes::FlowViewStyle;
using QtNodes::NodeStyle;
using QtNodes::ConnectionStyle;

int
main(int argc, char *argv[])
{

  QApplication app(argc, argv);

  MainWindow win;
  win.show();

  return app.exec();
}

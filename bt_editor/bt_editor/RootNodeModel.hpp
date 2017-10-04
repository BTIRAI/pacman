#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

#include <node_editor/NodeDataModel>
#include <iostream>
#include <memory>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;


class RootNodeModel : public NodeDataModel
{

public:
  RootNodeModel() {}

  virtual ~RootNodeModel() {}

public:

  bool captionVisible() const override
  { return true; }

public:

  unsigned int  nPorts(PortType portType) const override
  { return (portType==PortType::In) ? 0:1; }

  NodeDataType dataType(PortType portType, PortIndex portIndex) const override final
  { return NodeDataType {"", ""}; }

  std::shared_ptr<NodeData> outData(PortIndex port) override
  { return nullptr; }

  void setInData(std::shared_ptr<NodeData> data, int) override final {}

  int BTType()
  {
      return BT::ROOT;
  }

  QString get_line_edit()
  {
    return QString("ROOT");
  }


  std::unique_ptr<NodeDataModel> clone() const override
  { return  std::unique_ptr<NodeDataModel>( new RootNodeModel ); }

  QString caption() const override { return QString("Root"); }

  QString name() const override { return QString("Root"); }

  QWidget *embeddedWidget() override { return nullptr; }

  virtual ConnectionPolicy portOutConnectionPolicy(PortIndex /*portIndex*/) const {
      return ConnectionPolicy::One;
  }
};


#pragma once

#include <memory>

#include <QtWidgets/QWidget>

#include "PortType.hpp"
#include "NodeData.hpp"
#include "Serializable.hpp"
#include "NodeGeometry.hpp"
#include "NodePainterDelegate.hpp"
#include "NodeStyle.hpp"
#include "NodePainterDelegate.hpp"
#include "StyleCollection.hpp"
#include "Export.hpp"

namespace BT {
enum ReturnStatus {SUCCESS, FAILURE, RUNNING, IDLE, HALTED, EXIT};
enum DrawNodeType {PARALLEL, SELECTOR, SEQUENCE, SEQUENCESTAR, SELECTORSTAR, ACTION, CONDITION,DECORATOR, ROOT, SUBTREE};
}


namespace QtNodes
{

enum class NodeValidationState
{
  Valid,
  Warning,
  Error
};

class StyleCollection;
class NodeStyle;
class NodePainterDelegate;

class NodeDataModel
  : public QObject
  , public Serializable
{
  Q_OBJECT

public:

  NodeDataModel();

  virtual
  ~NodeDataModel() = default;

  /// Caption is used in GUI
  virtual QString
  caption() const = 0;

  /// It is possible to hide caption in GUI
  virtual bool
  captionVisible() const { return true; }

  /// Port caption is used in GUI to label individual ports
  virtual QString
  portCaption(PortType portType, PortIndex portIndex) const { return QString(""); }

  /// It is possible to hide port caption in GUI
  virtual bool
  portCaptionVisible(PortType portType, PortIndex portIndex) const { return false; }

  /// Name makes this model unique
  virtual QString
  name() const = 0;

  /// Function creates instances of a model stored in DataModelRegistry
  virtual std::unique_ptr<NodeDataModel>
  clone() const = 0;



public:

  QJsonObject
  save() const override;
  virtual int
  BTType() = 0;

  virtual
  QString get_line_edit() = 0;


public:

  virtual
  unsigned int nPorts(PortType portType) const = 0;

  virtual
  NodeDataType dataType(PortType portType, PortIndex portIndex) const = 0;

public:

  enum class ConnectionPolicy
  {
    One,
    Many,
  };

  virtual
  ConnectionPolicy
  portOutConnectionPolicy(PortIndex) const
  {
    return ConnectionPolicy::Many;
  }

  NodeStyle const&
  nodeStyle() const;

  void
  setNodeStyle(NodeStyle const& style);

public:

  /// Triggers the algorithm
  virtual
  void
  setInData(std::shared_ptr<NodeData> nodeData,
            PortIndex port) = 0;

  virtual
  std::shared_ptr<NodeData>
  outData(PortIndex port) = 0;

  virtual
  QWidget *
  embeddedWidget() = 0;

  virtual
  bool
  resizable() const { return false; }

  virtual
  NodeValidationState
  validationState() const { return NodeValidationState::Valid; }

  virtual
  QString
  validationMessage() const { return QString(""); }

  virtual
  NodePainterDelegate* painterDelegate() const { return nullptr; }

signals:

  void
  dataUpdated(PortIndex index);

  void
  dataInvalidated(PortIndex index);

  void
  computingStarted();

  void
  computingFinished();

private:

  NodeStyle _nodeStyle;
};
}

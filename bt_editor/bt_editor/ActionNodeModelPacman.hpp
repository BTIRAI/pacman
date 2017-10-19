#pragma once

#include "BehaviorTreeNodeModel.hpp"


class ActionNodeModel : public BehaviorTreeNodeModel
{
public:
    ActionNodeModel():
        BehaviorTreeNodeModel("Action", NodeFactory::get().getActionParameterModel() )
    { }
    int BTType()
    {
        return BT::ACTION;
    }
    virtual ~ActionNodeModel() {}

    virtual unsigned int  nPorts(PortType portType) const override
    { return (portType==PortType::In) ? 1:0; }

    virtual std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new ActionNodeModel ); }

    virtual QString name() const override { return QString("Action"); }
//    virtual QString name() const override { return QString("Action"); }
};



class EscapeNodeModel : public BehaviorTreeNodeModel
{
public:
    EscapeNodeModel():
        BehaviorTreeNodeModel("Escape", NodeFactory::get().getActionParameterModel() )
    { }
    int BTType()
    {
        return BT::ACTION;
    }
    virtual ~EscapeNodeModel() {}

    virtual unsigned int  nPorts(PortType portType) const override
    { return (portType==PortType::In) ? 1:0; }

    virtual std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new EscapeNodeModel ); }

    virtual QString name() const override { return QString("Escape"); }
};


class GreedyNodeModel : public BehaviorTreeNodeModel
{
public:
    GreedyNodeModel():
        BehaviorTreeNodeModel("Greedy", NodeFactory::get().getActionParameterModel() )
    { }
    int BTType()
    {
        return BT::ACTION;
    }
    virtual ~GreedyNodeModel() {}

    virtual unsigned int  nPorts(PortType portType) const override
    { return (portType==PortType::In) ? 1:0; }

    virtual std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new GreedyNodeModel ); }

    virtual QString name() const override { return QString("Greedy"); }
};


class KeepDistanceNodeModel : public BehaviorTreeNodeModel
{
public:
    KeepDistanceNodeModel():
        BehaviorTreeNodeModel("KeepDistance", NodeFactory::get().getActionParameterModel() )
    { }
    int BTType()
    {
        return BT::ACTION;
    }
    virtual ~KeepDistanceNodeModel() {}

    virtual unsigned int  nPorts(PortType portType) const override
    { return (portType==PortType::In) ? 1:0; }

    virtual std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new KeepDistanceNodeModel ); }

    virtual QString name() const override { return QString("KeepDistance"); }
};


class MoveToClosestDotNodeModel : public BehaviorTreeNodeModel
{
public:
    MoveToClosestDotNodeModel():
        BehaviorTreeNodeModel("MoveToClosestDot", NodeFactory::get().getActionParameterModel() )
    { }
    int BTType()
    {
        return BT::ACTION;
    }
    virtual ~MoveToClosestDotNodeModel() {}

    virtual unsigned int  nPorts(PortType portType) const override
    { return (portType==PortType::In) ? 1:0; }

    virtual std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new MoveToClosestDotNodeModel ); }

    virtual QString name() const override { return QString("MoveToClosestDot"); }
};


class ChaseNodeModel : public BehaviorTreeNodeModel
{
public:
    ChaseNodeModel():
        BehaviorTreeNodeModel("Chase", NodeFactory::get().getActionParameterModel() )
    { }
    int BTType()
    {
        return BT::ACTION;
    }
    virtual ~ChaseNodeModel() {}

    virtual unsigned int  nPorts(PortType portType) const override
    { return (portType==PortType::In) ? 1:0; }

    virtual std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new ChaseNodeModel ); }

    virtual QString name() const override { return QString("Chase"); }
};

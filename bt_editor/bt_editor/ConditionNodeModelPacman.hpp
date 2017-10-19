#pragma once

#include "BehaviorTreeNodeModel.hpp"


class ConditionNodeModel : public BehaviorTreeNodeModel
{
public:
    ConditionNodeModel():
        BehaviorTreeNodeModel("Condition", NodeFactory::get().getActionParameterModel() )
    { }
    int BTType()
    {
        return BT::CONDITION;
    }
    virtual ~ConditionNodeModel() {}

    virtual unsigned int  nPorts(PortType portType) const override
    { return (portType==PortType::In) ? 1:0; }

    virtual std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new ConditionNodeModel ); }

    virtual QString name() const override { return QString("Condition"); }
};



class IsCloseConditionNodeModel : public BehaviorTreeNodeModel
{
public:
    IsCloseConditionNodeModel():
        BehaviorTreeNodeModel("IsGhostClose", NodeFactory::get().getActionParameterModel() )
    { }
    int BTType()
    {
        return BT::CONDITION;
    }

    virtual ~IsCloseConditionNodeModel() {}

    virtual unsigned int  nPorts(PortType portType) const override
    { return (portType==PortType::In) ? 1:0; }

    virtual std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new IsCloseConditionNodeModel ); }

    virtual QString name() const override { return QString("IsGhostClose"); }
};


class IsVeryCloseConditionNodeModel : public BehaviorTreeNodeModel
{
public:
    IsVeryCloseConditionNodeModel():
        BehaviorTreeNodeModel("IsGhostVeryClose", NodeFactory::get().getActionParameterModel() )
    { }
    int BTType()
    {
        return BT::CONDITION;
    }

    virtual ~IsVeryCloseConditionNodeModel() {}

    virtual unsigned int  nPorts(PortType portType) const override
    { return (portType==PortType::In) ? 1:0; }

    virtual std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new IsVeryCloseConditionNodeModel ); }

    virtual QString name() const override { return QString("IsGhostVeryClose"); }
};


class IsScaredConditionNodeModel : public BehaviorTreeNodeModel
{
public:
    IsScaredConditionNodeModel():
        BehaviorTreeNodeModel("IsClosestGhostScared", NodeFactory::get().getActionParameterModel() )
    { }
    int BTType()
    {
        return BT::CONDITION;
    }

    virtual ~IsScaredConditionNodeModel() {}

    virtual unsigned int  nPorts(PortType portType) const override
    { return (portType==PortType::In) ? 1:0; }

    virtual std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new IsScaredConditionNodeModel ); }

    virtual QString name() const override { return QString("IsClosestGhostScared"); }
};

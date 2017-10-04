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



class IsGhostCloseConditionNodeModel : public BehaviorTreeNodeModel
{
public:
    IsGhostCloseConditionNodeModel():
        BehaviorTreeNodeModel("IsGhostClose", NodeFactory::get().getActionParameterModel() )
    { }
    int BTType()
    {
        return BT::CONDITION;
    }

    virtual ~IsGhostCloseConditionNodeModel() {}

    virtual unsigned int  nPorts(PortType portType) const override
    { return (portType==PortType::In) ? 1:0; }

    virtual std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new IsGhostCloseConditionNodeModel ); }

    virtual QString name() const override { return QString("IsGhostClose"); }
};

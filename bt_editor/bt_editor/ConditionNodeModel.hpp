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
        BehaviorTreeNodeModel("Is Ghost Close", NodeFactory::get().getActionParameterModel() )
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

    virtual QString name() const override { return QString("Is Ghost Close"); }
};

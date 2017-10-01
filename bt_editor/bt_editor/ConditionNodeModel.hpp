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



class YARPConditionNodeModel : public BehaviorTreeNodeModel
{
public:
    YARPConditionNodeModel():
        BehaviorTreeNodeModel("YARP Condition", NodeFactory::get().getActionParameterModel() )
    { }
    int BTType()
    {
        return BT::CONDITION;
    }

    virtual ~YARPConditionNodeModel() {}

    virtual unsigned int  nPorts(PortType portType) const override
    { return (portType==PortType::In) ? 1:0; }

    virtual std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new YARPConditionNodeModel ); }

    virtual QString name() const override { return QString("YARP Condition"); }
};

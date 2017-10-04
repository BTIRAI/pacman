#ifndef SUBTREE_NODEMODEL_HPP
#define SUBTREE_NODEMODEL_HPP

#include "BehaviorTreeNodeModel.hpp"

class SubtreeNodeModel : public BehaviorTreeNodeModel
{
public:
    SubtreeNodeModel():
        BehaviorTreeNodeModel("SubTree", NodeFactory::getSubtreeParameterModel() )
    {
      auto style = this->nodeStyle();
      style.NormalBoundaryColor = QColor(255,210,0);
      this->setNodeStyle(style);
    }

    virtual ~SubtreeNodeModel() {}

    int BTType()
    {
        return BT::SUBTREE;
    }

    virtual unsigned int  nPorts(PortType portType) const override
    { return (portType==PortType::In) ? 1:0; }


    virtual std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new SubtreeNodeModel ); }

    virtual QString name() const override { return QString("SubTree"); }
};


#endif // SUBTREE_NODEMODEL_HPP

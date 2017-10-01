#ifndef DECORATORNODEMODEL_HPP
#define DECORATORNODEMODEL_HPP

#include "BehaviorTreeNodeModel.hpp"

class DecoratorNodeModel : public BehaviorTreeNodeModel
{
public:
    DecoratorNodeModel():
        BehaviorTreeNodeModel("Decorator", NodeFactory::get().getDecoratorParameterModel() )
    {
      auto style = this->nodeStyle();
      style.NormalBoundaryColor = QColor(255,210,0);
      this->setNodeStyle(style);
    }

    virtual ~DecoratorNodeModel() {}
    int BTType()
    {
        return BT::DECORATOR;
    }

    unsigned int  nPorts(PortType portType) const override
    { return (portType==PortType::In) ? 1:1; }

    virtual ConnectionPolicy portOutConnectionPolicy(PortIndex /*portIndex*/) const {
        return ConnectionPolicy::One;
    }

    std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new DecoratorNodeModel ); }

    QString name() const override { return QString("Decorator"); }
};


#endif // DECORATORNODEMODEL_HPP

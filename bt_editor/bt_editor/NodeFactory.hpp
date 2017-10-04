#ifndef NODE_FACTORY_HPP
#define NODE_FACTORY_HPP

#include <QString>
#include <QWidget>
#include <functional>
#include <QDomDocument>



class NodeFactory
{
    NodeFactory() {}
public:
    typedef enum {ACTION, DECORATOR, SUBTREE} ModelType;

    struct ParamWidget{
        QString label;
        std::function<QWidget*()> instance_factory;
    };

    typedef std::map<QString, std::vector<ParamWidget>> ParametersModel;

    static void clear();

    static NodeFactory& get();

    static bool loadMetaModelFromFile(QString filename);

    static bool loadModelFromString(QString text);

    static bool loadMetaModelFromXML(const QDomDocument &document );

    static const ParametersModel& getActionParameterModel();

    static const ParametersModel& getDecoratorParameterModel();

    static const ParametersModel& getSubtreeParameterModel();

    static void addActionToModel(QString name, std::vector<ParamWidget> parameters_widgets);

    static void addDecoratorToModel(QString name, std::vector<ParamWidget> parameters_widgets);

    static std::function<QWidget*()> instanceFactoryText();
    static std::function<QWidget*()> instanceFactoryInt();
    static std::function<QWidget*()> instanceFactoryDouble();
    static std::function<QWidget*()> instanceFactoryCombo(QStringList options);


private:
    ParametersModel _action_parameter_model;
    ParametersModel _decorator_parameter_model;
    ParametersModel _subtree_parameter_model;

    void loadMetaModelPart1(const QDomElement& metamodel_root);
};

#endif // NODE_FACTORY_HPP

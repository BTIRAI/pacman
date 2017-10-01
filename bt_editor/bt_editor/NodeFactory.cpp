#include "NodeFactory.hpp"
#include <QFile>
#include <QDomElement>
#include <QLineEdit>
#include <QComboBox>
#include <QDebug>

bool NodeFactory::loadMetaModelFromFile(QString filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        throw std::runtime_error(std::string("can't laod the file ") + filename.toStdString());
    }
    QString errorStr;
    int errorLine, errorColumn;

    QDomDocument domDocument;

    if (!domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
    {
        QString error_msg = QString("XML Parse error of file %1 at line %2: %3")
                                 .arg( filename).arg(errorLine).arg(errorStr);
        throw std::runtime_error(error_msg.toStdString());
    }
    //loadMetaModelFromXML(domDocument);
    return true;
}

bool NodeFactory::loadModelFromString(QString text)
{
    QString errorStr;
    int errorLine, errorColumn;

    QDomDocument domDocument;

    if (!domDocument.setContent(text, true, &errorStr, &errorLine, &errorColumn))
    {
        QString error_msg = QString("XML Parse error at line %1: %2").arg(errorLine).arg(errorStr);
        throw std::runtime_error(error_msg.toStdString());
    }

   // QDomNodeList root_children = domDocument.childNodes();

   // loadMetaModelFromXML(root);
    return true;
}

bool NodeFactory::loadMetaModelFromXML(const QDomDocument& domDocument)

{
    QDomElement root = domDocument.documentElement();
    QDomElement metamodel_root = root.firstChildElement("BehaviorTreeMetaModel");

    //-------------------------------------------------
    // READ the metamodel section if present
    if( !metamodel_root.isNull() )
    {
        NodeFactory::get().loadMetaModelPart1(metamodel_root);
    }

    //-------------------------------------------------
    // Recursively add any action, subtree or decorator that is missing. Note that no ParamWidget can be added
    std::function<void(const QDomElement&)> recursiveStep;

    recursiveStep = [&](const QDomElement& element)
    {
        const QString element_type = element.tagName();
        if( element_type == "BehaviorTreeMetaModel")
        {
            return; //skip this, we did it already.
        }

        ParametersModel* parameters = nullptr;

        if( element_type == "Action")
        {
            parameters = &NodeFactory::get()._action_parameter_model;
        }
        else if( element_type == "Decorator")
        {
            parameters = &NodeFactory::get()._decorator_parameter_model;
        }
        else if( element_type == "SubTreeDefinition")
        {
            parameters = &NodeFactory::get()._subtree_parameter_model;
        }

        if( parameters )
        {
            const QString ID = element.attribute("ID");
            if(  parameters->count(ID) == 0)
            {
                (*parameters)[ID] = std::vector<ParamWidget>(); //empty one
                qDebug() << "Warning: adding automatically the metamodel to create ["
                         <<  ID << "], but some key features might be missing.\n"
                         << "Please use <BehaviorTreeMetaModel> instead.";
            }
        }

        for(QDomElement child = element.firstChildElement();
            child.isNull() == false;
            child = child.nextSiblingElement() )
        {
            recursiveStep(child);
        }
    };

    //start recursion
    recursiveStep(root);

    return true;
}

void NodeFactory::loadMetaModelPart1(const QDomElement& metamodel_root)
{
    const QString type_name[3]  = { "Action", "Decorator", "SubTree" };
    const QString type_group[3] = { "Actions", "Decorators", "SubTrees" };
    ParametersModel* parameter_model[3] = { &NodeFactory::get()._action_parameter_model,
                                            &NodeFactory::get()._decorator_parameter_model,
                                            &NodeFactory::get()._subtree_parameter_model};
    for(int i=0; i<3; i++)
    {
        QDomElement group = metamodel_root.firstChildElement( type_group[i] );

        if ( group.isNull() ) {
           qDebug() << QString("Potential ERROR: No <%1> element found in the XML file!")
                       .arg(type_group[i]);
           continue;
        }

        for (QDomElement  elem_action = group.firstChildElement( type_name[i] )  ;
             elem_action.isNull() == false;
             elem_action = elem_action.nextSiblingElement( type_name[i] ) )
        {
            if( !elem_action.hasAttribute("ID") )
            {
                throw  std::runtime_error("element <Action>must have the attribute 'ID'");
            }

            QString name = elem_action.attribute("ID");
            (*parameter_model[i])[name] = {};

            for (QDomElement  elem_parameter = elem_action.firstChildElement(  "Parameter" )  ;
                 elem_parameter.isNull() == false;
                 elem_parameter = elem_parameter.nextSiblingElement( "Parameter" ) )
            {
                if( !elem_parameter.hasAttribute("label") || !elem_parameter.hasAttribute("type") )
                {
                    throw  std::runtime_error("element <Parameter> needs an attribute 'type' and 'label'");
                }
                ParamWidget pw;
                pw.label = elem_parameter.attribute("label");
                QString widget_type = elem_parameter.attribute("type");

                if( widget_type == "Text"){
                    pw.instance_factory = instanceFactoryText();
                }
                else if( widget_type == "Int"){
                    pw.instance_factory = instanceFactoryInt();
                }
                else if( widget_type == "Double"){
                    pw.instance_factory = instanceFactoryDouble();
                }
                else if( widget_type == "Combo"){

                    if( !elem_parameter.hasAttribute("options"))
                    {
                        throw  std::runtime_error("A parameter marked ad Combo must have the field [options]");
                    }

                    QString options = elem_parameter.attribute("options");
                    QStringList option_list = options.split(";", QString::SkipEmptyParts);

                    pw.instance_factory = instanceFactoryCombo(option_list);

                }
                else{
                    throw  std::runtime_error("Attribute 'type' of element <Parameter>"
                                              " must be either: Text, Int, Double or Combo");
                }
                (*parameter_model[i])[name].push_back( pw );
            }
        }
    }
}



const NodeFactory::ParametersModel &NodeFactory::getActionParameterModel()
{
    return NodeFactory::get()._action_parameter_model;
}

const NodeFactory::ParametersModel &NodeFactory::getDecoratorParameterModel()
{
  return NodeFactory::get()._decorator_parameter_model;
}

const NodeFactory::ParametersModel &NodeFactory::getSubtreeParameterModel()
{
  return NodeFactory::get()._subtree_parameter_model;
}

std::function<QWidget *()> NodeFactory::instanceFactoryText()
{
  return [](){
    QLineEdit* line = new QLineEdit();
    line->setAlignment( Qt::AlignHCenter);
    return line;
  };
}

std::function<QWidget *()> NodeFactory::instanceFactoryInt()
{
  return [](){
    QLineEdit* line = new QLineEdit();
    line->setValidator( new QIntValidator( line ));
    line->setAlignment( Qt::AlignHCenter);
    return line;
  };
}

std::function<QWidget *()> NodeFactory::instanceFactoryDouble()
{
  return [](){
    QLineEdit* line = new QLineEdit();
    line->setValidator( new QDoubleValidator( line ));
    line->setAlignment( Qt::AlignHCenter);
    return line;
  };
}

std::function<QWidget *()> NodeFactory::instanceFactoryCombo(QStringList options)
{
  return [options](){
    QComboBox* combo = new QComboBox();
    combo->addItems(options);
    return combo;
  };
}

void NodeFactory::clear()
{
    NodeFactory::get()._action_parameter_model.clear();
    NodeFactory::get()._decorator_parameter_model.clear();
}

NodeFactory &NodeFactory::get(){
    static NodeFactory instance;
    return instance;
}

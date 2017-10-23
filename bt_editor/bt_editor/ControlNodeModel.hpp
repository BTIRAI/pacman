#pragma once

#include <QtCore/QObject>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

#include <node_editor/NodeDataModel>
#include <iostream>
#include <memory>
#include <QXmlStreamWriter>
using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;


class ControlNodeModel : public NodeDataModel
{

public:
    ControlNodeModel()
    {
        _main_widget = new QWidget;
        _label = new QLabel( _main_widget );
        _name_edit = new QLineEdit( _main_widget );

        _name_edit->setFixedWidth(50);


        QHBoxLayout *layout = new QHBoxLayout( _main_widget );
        layout->addWidget( _label );
        layout->addWidget( _name_edit );

        _name_edit->setHidden(true);



        QFont font = _label->font();
        font.setPointSize(10);
        font.setBold(true);
        _label->setFont(font);

        QPalette palette = _label->palette();
        palette.setColor(_label->foregroundRole(), Qt::white);
        _label->setPalette(palette);

        _main_widget->setAttribute(Qt::WA_NoSystemBackground);
        _main_widget->setStyleSheet("background-color: transparent");
        _main_widget->setLayout( layout );
        _name_edit->setStyleSheet("background-color: white");
    }

    virtual ~ControlNodeModel() {}

public:

    QJsonObject save() const override
    {
        QJsonObject nodeJson;
        nodeJson["ID"] = _name_edit->text();
        nodeJson["type"] = name();
        return nodeJson;
    }

    void restore(QJsonObject const &nodeJson) override
    {
        QJsonValue v = nodeJson["name"];
        if (!v.isUndefined()) {
            _name_edit->setHidden(false);
            _name_edit->setText( v.toString() );
        }
        else{
          _name_edit->setHidden(true);
        }
    }

    void saveXML(QXmlStreamWriter& stream)
    {
        stream.writeStartElement( this->name() );
        stream.writeTextElement("ID", _name_edit->text() );
    }

    void lock( bool locked)
    {
      _name_edit->setReadOnly( locked );
    }

public:

    bool captionVisible() const override
    { return false; } 

    QString caption() const override final { return _name_edit->text(); }

    bool portCaptionVisible(PortType , PortIndex ) const override
    { return false; }

    unsigned int  nPorts(PortType portType) const override
    { return 1; }

    NodeDataType dataType(PortType portType, PortIndex portIndex) const override final
    { return NodeDataType {"", ""}; }

    std::shared_ptr<NodeData> outData(PortIndex port) override
    { return nullptr; }

    void setInData(std::shared_ptr<NodeData> data, int) override final {}

    QWidget *embeddedWidget() override { return _main_widget; }
protected:
    QLineEdit* _name_edit;
    QLabel*    _label;
    QWidget*   _main_widget;
    int _bt_node_type;
};
//------------------------------------------------

template <typename T>
class ControlNodeModelBase : public ControlNodeModel
{
public:
    ControlNodeModelBase(): ControlNodeModel()
    {
        _label->setText( this->name() + ":" );
    }
    virtual ~ControlNodeModelBase() {}
    virtual int BTType() = 0;

    QString name() const override { return T::staticName(); }


    QString get_line_edit()
    {
      return QString("IFTHENELSE");
    }

    std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new T ); }
};

//-------------------------------------------------
class SequenceModel: public ControlNodeModelBase<SequenceModel>
{
public:
    SequenceModel(): ControlNodeModelBase() {  _bt_node_type = BT::SEQUENCE;  std::cout << "Sequence Created: " << _bt_node_type << std::endl;}
    virtual ~SequenceModel() {}
    int BTType()
    {
        return BT::SEQUENCE;
    }
    static QString staticName() { return QString("Sequence"); }
};


class SelectorModel: public  ControlNodeModelBase<SelectorModel>
{
public:
    SelectorModel(): ControlNodeModelBase() { }
    int BTType()
    {
        return BT::SELECTOR;
    }
    virtual ~SelectorModel() {}
    static QString staticName() { return QString("Fallback"); }
};


class SequenceStarModel: public ControlNodeModelBase<SequenceStarModel>
{
public:
    SequenceStarModel(): ControlNodeModelBase() { }
    int BTType()
    {
        return BT::SEQUENCESTAR;
    }
    virtual ~SequenceStarModel() {}
    static QString staticName() { return QString("SequenceStar"); }
};



class IfThenElseModel: public  ControlNodeModelBase<IfThenElseModel>
{
public:
    IfThenElseModel(): ControlNodeModelBase() { }
    int BTType()
    {
        return BT::DECORATOR;
    }
    virtual ~IfThenElseModel() {}
    static QString staticName() { return QString("IfThenElse"); }
};




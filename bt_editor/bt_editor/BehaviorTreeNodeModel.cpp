#include "ActionNodeModel.hpp"
#include "ConditionNodeModel.hpp"
#include <QTextEdit>
#include <QBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDebug>
#include <QFile>
#include <QDomDocument>

#include <iostream>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define NOMINMAX
#include <windows.h>
#undef min
#undef max
#else
#include  <dirent.h> //dirent would also work for windows to find file in a directory but <windows.h> has a better font
#endif


#include <fstream>

QStringList get_all_files_names_within_folder(std::string folder, std::string type)
{

    QStringList names;
    std::string search_path = folder + "/"+type+"*.lua";

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    //OS is windows
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
    if(hFind != INVALID_HANDLE_VALUE) {
        do {
            // read all (real) files in current folder
            // , delete '!' read other 2 default folder . and ..
            if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
                names.push_back(QString(fd.cFileName));
            }
        }while(::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }

#else
    //OS is unix

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (search_path)) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            names.push_back(QString(ent->d_name));
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
    }
#endif
    return names;
}



BehaviorTreeNodeModel::BehaviorTreeNodeModel(QString name,
                                             const NodeFactory::ParametersModel& parameter_model):
    _parameter_model(parameter_model)
{


    _main_widget = new QWidget;
        _label = new QLabel( _main_widget );
        //_ID_selection_combobox = new QComboBox(_main_widget);
        std::cout << "combobox created" << std::endl;
        _label->setText( name );

        QVBoxLayout *main_layout = new QVBoxLayout( _main_widget );
        QHBoxLayout *top_layout  = new QHBoxLayout(  );
        //_line_edit = new QLineEdit(_main_widget);
        //_text_edit = new QTextEdit (_main_widget);

        //_text_edit->setTextColor(Qt::black);
        //_ID_selection_combobox->setStyleSheet("color: black; background-color: white");


        //_line_edit->setStyleSheet("background-color: white");

        _params_widget = new QWidget( _main_widget );
        _form_layout = new QFormLayout( _params_widget );

        top_layout->addWidget( _label );
        //top_layout->addWidget( _ID_selection_combobox );

        main_layout->addLayout(top_layout);
        //main_layout->addWidget(_params_widget);
        //main_layout->addWidget(_line_edit);
        //main_layout->addWidget(_text_edit);

        //QStringList combo_items = get_all_files_names_within_folder(".", name.toStdString());



        //_ID_selection_combobox->addItems(combo_items);
    QFont font = _label->font();
    font.setPointSize(10);
    font.setBold(true);
    _label->setFont(font);

    QPalette palette = _label->palette();
    palette.setColor(_label->foregroundRole(), Qt::white);
    _label->setPalette(palette);

    _main_widget->setAttribute(Qt::WA_NoSystemBackground);
    _main_widget->setLayout( main_layout );

    _form_layout->setHorizontalSpacing(5);
    _form_layout->setVerticalSpacing(2);
    _form_layout->setContentsMargins(0, 0, 0, 0);
    _params_widget->setStyleSheet("color: white;");

    main_layout->setMargin(0);
    _main_widget->setStyleSheet("background-color: transparent; color: white; ");


}

QString BehaviorTreeNodeModel::caption() const {
    return type();
}

QString BehaviorTreeNodeModel::type() const
{
    return _ID_selection_combobox->currentText();
}

QString BehaviorTreeNodeModel::get_line_edit()
{
    return _line_edit->text();
}


QString BehaviorTreeNodeModel::get_text_edit()
{
    return _text_edit->toPlainText();
}

void BehaviorTreeNodeModel::lastComboItem() const
{
    // TODO DO IT!
    std::cout << "Setting combobox item" << std::endl;
    // ID_selection_combobox->isEnabled();
    std::cout << "combobox item SET" << std::endl;
}


std::vector<std::pair<QString, QString>> BehaviorTreeNodeModel::getCurrentParameters() const
{
    std::vector<std::pair<QString, QString>> out;

    for(int row = 0; row < _form_layout->rowCount(); row++)
    {
        auto label_item = _form_layout->itemAt(row, QFormLayout::LabelRole);
        auto field_item  = _form_layout->itemAt(row, QFormLayout::FieldRole);
        if( label_item && field_item )
        {
            QLabel* label = static_cast<QLabel*>( label_item->widget() );

            QLineEdit* line  = dynamic_cast<QLineEdit*>( field_item->widget() );
            QComboBox* combo = dynamic_cast<QComboBox*>( field_item->widget() );
            if( line ){
                out.push_back( { label->text(), line->text() } );
            }
            else if( combo )
            {
                out.push_back( { label->text(), combo->currentText() } );
            }
            break;
        }
    }
    return out;
}

QJsonObject BehaviorTreeNodeModel::save() const
{
    QJsonObject nodeJson;
    nodeJson["type"] = type();
    nodeJson["ID"] = caption();
    return nodeJson;
}

void BehaviorTreeNodeModel::restore(std::map<QString,QString> attributes)
{
    // we expect to find at least two attributes, "name" and "ID".
    // Other nodes represent parameters.

    std::cout << "restoring: " << std::endl;
    {
//        auto v_type = attributes.find("ID");

//        if (  v_type == attributes.end() )
//        {
//            throw std::runtime_error("the TreeNodeModel needs a [ID] to be restored");
//        }
//        const QString type_name = v_type->second;

//        _ID_selection_combobox->setCurrentText( type_name );
//        onComboBoxUpdated( type_name );
//        attributes.erase(v_type);
        //--------------------------
        auto v_name = attributes.find("name");
        if (  v_name != attributes.end() )
        {
            _ID = v_name->second;
            attributes.erase(v_name);
        }
        else{
            //_ID = v_type->second;
        }
    }

    for (auto it: attributes)
    {
        const QString& param_name = it.first;
        const QString& value      = it.second;

        bool found = false;
        for(int row = 0; row < _form_layout->rowCount(); row++)
        {
            auto label_item  = _form_layout->itemAt(row, QFormLayout::LabelRole);
            auto field_item  = _form_layout->itemAt(row, QFormLayout::FieldRole);
            if( label_item && field_item )
            {
                QLabel* label = static_cast<QLabel*>( label_item->widget() );
                //-----------------------------
                if(label->text() == param_name)
                {
                    QLineEdit* line  = dynamic_cast<QLineEdit*>( field_item->widget() );
                    QComboBox* combo = dynamic_cast<QComboBox*>( field_item->widget() );
                    if( line ){
                        line->setText(value);
                        found = true;
                    }
                    else if( combo )
                    {
                        int res = combo->findText(value);
                        if( res < 0) {
                            qDebug() << "Error: the combo ["<< param_name <<
                                        "] does not contain the value '"<< value ;
                        }
                        else {
                            combo->setCurrentText(value);
                            found = true;
                        }
                    }
                    break;
                }
            } // end for
            if( !found )
            {
                qDebug() << "Error: the parameter ["<< param_name <<
                            "] is not present in " << name() << " / " << type();
            }
        }
    }
}

void BehaviorTreeNodeModel::restore(const QJsonObject &nodeJson)
{
    std::map<QString,QString> attributes;
    for(auto it = nodeJson.begin(); it != nodeJson.end(); it++ )
    {
        attributes.insert( std::make_pair(it.key(), it.value().toString()) );
    }
    restore(attributes);
}

void BehaviorTreeNodeModel::lock(bool locked)
{
    _ID_selection_combobox->setEnabled( !locked );
    _text_edit->setEnabled(!locked);

//    for(int row = 0; row < _form_layout->rowCount(); row++)
//    {
//        auto field_item  = _form_layout->itemAt(row, QFormLayout::FieldRole);

//        if( field_item )
//        {
//            QLineEdit* line  = dynamic_cast<QLineEdit*>( field_item->widget() );
//            QComboBox* combo = dynamic_cast<QComboBox*>( field_item->widget() );
//            if( line ){
//                line->setReadOnly( locked );
//            }
//            else if( combo )
//            {
//                combo->setEnabled( !locked );
//            }
//        }
//    }
}

//void BehaviorTreeNodeModel::onComboBoxUpdated(QString item_text)
//{
//  while ( _form_layout->count() != 0)
//  {
//    QLayoutItem *forDeletion = _form_layout->takeAt(0);
//    delete forDeletion->widget();
//    delete forDeletion;
//  }

//  int row = 0;
//  //TODO: check if found
//  const auto& params = _parameter_model.at(item_text);
//  _params_widget->setVisible( params.size() > 0);

//  for(const NodeFactory::ParamWidget& param: params )
//  {
//    QWidget* field_widget = param.instance_factory();
//    field_widget->setStyleSheet("color: white; background-color: gray; border: 1px solid #FFFFFF");

//    _form_layout->setWidget(row,  QFormLayout::LabelRole, new QLabel( param.label, _params_widget ));
//    _form_layout->setWidget(row,  QFormLayout::FieldRole, field_widget);
//    row++;
//  }
//  _params_widget->adjustSize();
//  _main_widget->adjustSize();

//  emit adjustSize();
//}

void BehaviorTreeNodeModel::onComboBoxUpdated(QString item_text)
{
    std::ifstream file(item_text.toStdString());
    std::string str;
    std::string file_contents;
    while (std::getline(file, str))
    {
        file_contents += str;
        file_contents.push_back('\n');
    }
    _text_edit->setText(file_contents.c_str());
}

void BehaviorTreeNodeModel::onTextBoxUpdated()
{
//    std::cout << "not saving file, edit BehaviorTreeNodeModel::onTextBoxUpdated" << std::endl;

//    return;

    std::ofstream myfile;
    myfile.open (_ID_selection_combobox->currentText().toStdString().c_str());
    myfile << _text_edit->toPlainText().toStdString().c_str();
    myfile.close();

    std::cout << "new data added" << std::endl;
}



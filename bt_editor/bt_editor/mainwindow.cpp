#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QList>
#include <QMessageBox>
#include <QFileDialog>
#include <node_editor/Node>
#include <node_editor/NodeData>
#include <node_editor/NodeStyle>
#include <node_editor/FlowScene>
#include <node_editor/FlowView>
#include <node_editor/DataModelRegistry>

#include "XmlParsers.hpp"
#include "ControlNodeModel.hpp"
#include "RootNodeModel.hpp"
#include "DecoratorNodeModel.hpp"
#include "SubtreeNodeModel.hpp"
#include "ActionNodeModelPacman.hpp"
#include "ConditionNodeModelPacman.hpp"

#include "utils.h"

#include "QInputDialog"

#include "QErrorMessage"

#include <fstream>

using QtNodes::DataModelRegistry;
using QtNodes::FlowView;
using QtNodes::FlowScene;
using QtNodes::NodeGraphicsObject;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _arrange_shortcut(QKeySequence(Qt::CTRL + Qt::Key_A), this),
    _root_node(nullptr)
{
    ui->setupUi(this);




    QSettings settings("Michele Colledanchise", "BehaviorTreeEditor");
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    restoreState(settings.value("MainWindow/windowState").toByteArray());

    auto ret = std::make_shared<DataModelRegistry>();

    ret->registerModel<RootNodeModel>("Root");
    ret->registerModel<SequenceModel>("Control");
    //ret->registerModel<SequenceStarModel>("Control");
    ret->registerModel<SelectorModel>("Control");

    ret->registerModel<GreedyNodeModel>("Action");
    ret->registerModel<EscapeNodeModel>("Action");
    ret->registerModel<ChaseNodeModel>("Action");
    ret->registerModel<MoveToClosestDotNodeModel>("Action");
    ret->registerModel<KeepDistanceNodeModel>("Action");

    ret->registerModel<IsCloseConditionNodeModel>("Condition");
    ret->registerModel<IsVeryCloseConditionNodeModel>("Condition");
    ret->registerModel<IsScaredConditionNodeModel>("Condition");



    ret->registerModel<DecoratorNodeModel>("Decorator");

    _main_scene = new FlowScene( ret );
    _main_view  = new FlowView( _main_scene );

    ui->tabWidget->addTab( _main_view, "BehaviorTree" );

    this->setMenuBar(ui->menubar);
    ui->menubar->setNativeMenuBar(false);

    connect( &_arrange_shortcut, &QShortcut::activated,
             this,   &MainWindow::onNodeMoved  );

    connect( _main_scene, &QtNodes::FlowScene::changed,
             this,   &MainWindow::onSceneChanged  );

    connect( this, SIGNAL(updateGraphic()),  _main_scene,  SLOT(updateGraphic())  );
    connect( this, SIGNAL(updateGraphic()),  _main_view,   SLOT(updateGraphic())  );
    connect( this, SIGNAL(updateGraphic()),  _main_view,   SLOT(repaint())  );
    connect( &_periodic_timer, SIGNAL(timeout()), this, SLOT(onTimerUpdate()) );

    _periodic_timer.start(10);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::loadFromXML(const QString& xml_text)
{
    QString errorStr;
    int errorLine, errorColumn;

    QDomDocument domDocument;

    if (!domDocument.setContent(xml_text, true, &errorStr, &errorLine, &errorColumn))
    {
        QString error_msg = QString("XML Parse error at line %1: %2").arg(errorLine).arg(errorStr);
        throw std::runtime_error(error_msg.toStdString());
    }

    QDomElement root = domDocument.documentElement();

    if( root.tagName() != "root" )
    {
        throw std::runtime_error("Expecting the node <root> in the XML");
    }

    QDomElement  behaviortree_root = root.firstChildElement("BehaviorTree");
    QDomElement loosenodes_root = root.firstChildElement("LooseNodes");



    QDomNodeList loosenodes = loosenodes_root.childNodes();

    NodeFactory::loadMetaModelFromXML(domDocument);

    _main_scene->clearScene();
    QtNodes::Node& first_qt_node = _main_scene->createNode( _main_scene->registry().create("Root"));

    std::cout<<	"Starting parsing"<< std::endl;
    ParseBehaviorTreeXML(behaviortree_root, _main_scene, first_qt_node);
    std::cout<<	"Starting parsing " <<  loosenodes.size() << " loose nodes"<< std::endl;

    ParseLooseNodesXML(loosenodes_root, _main_scene, first_qt_node);

    std::cout<<"XML Parsed Successfully!"<< std::endl;

    NodeReorder( *_main_scene );

//    for (auto& it: _main_scene->nodes() )
//    {
//        QtNodes::Node* node = it.second.get();

//        ActionNodeModel* action_model = dynamic_cast<ActionNodeModel*>( node->nodeDataModel() );
//        if( action_model )
//        {
//            connect( action_model, &ActionNodeModel::adjustSize, this, &MainWindow::onNodeSizeChanged);
//        }
//    }

//    lockEditing( ui->playButton->isChecked() );
}



void MainWindow::on_actionLoad_triggered()
{
    QSettings settings("Michele Colledanchise", "BehaviorTreeEditor");
    QString directory_path  = settings.value("MainWindow.lastLoadDirectory",
                                             QDir::homePath() ).toString();

    QString fileName = QFileDialog::getOpenFileName(nullptr,
                                                    tr("Open Flow Scene"), directory_path,
                                                    tr("XML StateMachine Files (*.xml)"));
    if (!QFileInfo::exists(fileName)){
        return;
    }

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)){
        return;
    }

    directory_path = QFileInfo(fileName).absolutePath();
    settings.setValue("MainWindow.lastLoadDirectory", directory_path);
    settings.sync();

    QString xml_text;

    QTextStream in(&file);
    while (!in.atEnd()) {
        xml_text += in.readLine();
    }

    loadFromXML(xml_text);

}

void MainWindow::recursivelyCreateXml(QDomDocument& doc, QDomElement& parent_element, const QtNodes::Node* node)
{
    const QtNodes::NodeDataModel* node_model = node->nodeDataModel();
    const QString model_name = node_model->name();
    QDomElement element = doc.createElement( model_name );

    parent_element.appendChild( element );


    auto node_children = getChildren(*_main_scene, *node );
    for(QtNodes::Node* child : node_children)
    {
        recursivelyCreateXml(doc, element, child );
    }
}


void MainWindow::on_actionSave_triggered()
{

    int i = 0;

    std::vector<QtNodes::Node*> roots = findRoots( *_main_scene );

    if( roots.empty())
    {
        int ret = QMessageBox::warning(this, tr("Oops!"),
                                       tr("There are no nodes to save"),
                                       QMessageBox::Ok);
        return;
    }


    std::vector<QtNodes::Node*> loose_nodes = findRoots( *_main_scene );

    QtNodes::Node* root;

    bool has_root = false;

    for (QtNodes::Node* node : roots )
    {
        if(node->nodeDataModel()->name() == "Root")
        {
            loose_nodes.erase(std::remove(loose_nodes.begin(), loose_nodes.end(), node), loose_nodes.end());
            root = node;
            has_root = true;
            break;
        }
    }

    if( has_root && getChildren(*_main_scene, *root).empty())
    {
        int ret = QMessageBox::warning(this, tr("Oops!"),
                                       tr("The Root has no children"),
                                       QMessageBox::Ok);
        return;
    }


    //----------------------------
    QDomDocument doc;
    QDomElement root_element = doc.createElement( "root" );
    doc.appendChild( root_element );

    if(has_root)
    {
        qDebug() << "HAS ROOT\n";
        QDomElement bt_node = doc.createElement( "BehaviorTree" );
        root_element.appendChild(bt_node);

        auto root_children = getChildren(*_main_scene, *root );
        if(!root_children.empty())
        {
            qDebug() << "recursivelyCreateXml\n";

            recursivelyCreateXml(doc, bt_node, root_children.front() );

            qDebug() << "recursivelyCreateXmlDONE\n";


        }
    }

    if(!loose_nodes.empty())
    {
        QDomElement loose_nodes_xml = doc.createElement( "LooseNodes" );
        root_element.appendChild(loose_nodes_xml);
        for(QtNodes::Node* loose_node : loose_nodes)
        {
            recursivelyCreateXml(doc, loose_nodes_xml, loose_node );
        }

    }
    //-------------------------------------
    QSettings settings("Michele Colledanchise", "BehaviorTreeEditor");
    QString directory_path  = settings.value("MainWindow.lastSaveDirectory",
                                             QDir::currentPath() ).toString();

    QFileDialog saveDialog;
    saveDialog.setAcceptMode(QFileDialog::AcceptSave);
    saveDialog.setDefaultSuffix("xml");
    saveDialog.setNameFilter("XML FILE (*.xml)");
    saveDialog.setDirectory(directory_path);
    saveDialog.exec();

    QString fileName;
    if(saveDialog.result() == QDialog::Accepted && saveDialog.selectedFiles().size() == 1)
    {
        fileName = saveDialog.selectedFiles().at(0);
    }

    if (fileName.isEmpty()){
        return;
    }

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << doc.toString(4) << endl;
    }

    directory_path = QFileInfo(fileName).absolutePath();
    settings.setValue("MainWindow.lastSaveDirectory", directory_path);
}


void MainWindow::createPacmanXml()
{

    int i = 0;
    std::vector<QtNodes::Node*> roots = findRoots( *_main_scene );


    std::vector<QtNodes::Node*> loose_nodes = findRoots( *_main_scene );

    QtNodes::Node* root;

    bool has_root = false;

    for (QtNodes::Node* node : roots )
    {
        if(node->nodeDataModel()->name() == "Root")
        {
            loose_nodes.erase(std::remove(loose_nodes.begin(), loose_nodes.end(), node), loose_nodes.end());
            root = node;
            has_root = true;
            break;
        }
    }

    //----------------------------
    QDomDocument doc;
    QDomElement root_element = doc.createElement( "root" );
    doc.appendChild( root_element );

    if(has_root)
    {
        QDomElement bt_node = doc.createElement( "BehaviorTree" );
        root_element.appendChild(bt_node);

        auto root_children = getChildren(*_main_scene, *root );
        if(!root_children.empty())
        {
            recursivelyCreateXml(doc, bt_node, root_children.front() );
        }
    }

    if(!loose_nodes.empty())
    {
        QDomElement loose_nodes_xml = doc.createElement( "LooseNodes" );
        root_element.appendChild(loose_nodes_xml);
        for(QtNodes::Node* loose_node : loose_nodes)
        {
            recursivelyCreateXml(doc, loose_nodes_xml, loose_node );
        }
    }

    QFile file("pacmantree.xml");
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << doc.toString(4) << endl;
    }
}


void MainWindow::on_actionZoom_In_triggered()
{
    _main_view->scaleDown();
}

void MainWindow::on_actionZoom_ut_triggered()
{
    _main_view->scaleUp();
}

void MainWindow::on_actionAuto_arrange_triggered()
{
    NodeReorder( *_main_scene );
}

void MainWindow::onNodeMoved()
{
    NodeReorder( *_main_scene );
}

void MainWindow::onNodeSizeChanged()
{
    for (auto& it: _main_scene->nodes() )
    {
        QtNodes::Node* node = it.second.get();

        node->nodeGeometry().recalculateSize();
        node->nodeGraphicsObject().update();
    }
    _main_scene->update();
    NodeReorder( *_main_scene );
}

void MainWindow::onSceneChanged()
{
    //qDebug() << "onSceneChanged " ;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings("Michele Colledanchise", "BehaviorTreeEditor");
    settings.setValue("MainWindow/geometry", saveGeometry());
    settings.setValue("MainWindow/windowState", saveState());
    QMainWindow::closeEvent(event);
}


void MainWindow::updateStates(QXmlInputSource* source)
{
    std::unique_lock<std::mutex> lock(_mutex);

    if( !_root_node ) return;

    for (auto& it: _main_scene->nodes())
    {
        it.second->nodeGraphicsObject().setGeometryChanged();
    }

    QXmlSimpleReader parser;
    StateUpdateXmlHandler handler(_main_scene, _root_node);

    parser.setContentHandler( &handler );

    std::cout<<	"Start parsing"<< std::endl;

    if(parser.parse(source))
    {
        std::cout<<"Parsed Successfully!"<< std::endl;
    }
    else {
        std::cout<<"Parsing Failed..."  << std::endl;
    }

    for (auto& it: _main_scene->nodes())
    {
        it.second->nodeGraphicsObject().update();
    }
}

void MainWindow::lockEditing(bool locked)
{
    if( locked)
    {
        std::vector<QtNodes::Node*> roots = findRoots( *_main_scene );
        bool valid_root = (roots.size() == 1) && ( dynamic_cast<RootNodeModel*>(roots.front()->nodeDataModel() ));

        if( valid_root) _root_node = roots.front();
    }

    for (auto& it: _main_scene->nodes() )
    {
        QtNodes::Node* node = it.second.get();
        node->nodeGraphicsObject().lock( locked );

        BehaviorTreeNodeModel* bt_model = dynamic_cast<BehaviorTreeNodeModel*>( node->nodeDataModel() );
        if( bt_model )
        {
            bt_model->lock(locked);
        }
        else
        {
            ControlNodeModel* ctr_model = dynamic_cast<ControlNodeModel*>( node->nodeDataModel() );
            if( ctr_model )
            {
                ctr_model->lock(locked);
            }
        }
        if( !locked )
        {
            node->nodeGraphicsObject().setGeometryChanged();
            node->nodeDataModel()->setNodeStyle( QtNodes::StyleCollection::nodeStyle() );
            node->nodeGraphicsObject().update();
        }
    }

    for (auto& it: _main_scene->connections() )
    {
        QtNodes::Connection* conn = it.second.get();
        conn->getConnectionGraphicsObject().lock( locked );
    }

    if( !locked ){
        emit updateGraphic();
    }
}

void MainWindow::on_selectMode_sliderPressed()
{

}

void MainWindow::on_selectMode_valueChanged(int value)
{

}




void MainWindow::on_actionAdd_Action_triggered()
{
    bool ok;
    std::string filename;
    QString text = QInputDialog::getText(0, "New Action",
                                         "filename:", QLineEdit::Normal,
                                         "", &ok);

    filename = text.toStdString();
    if (ok && !text.isEmpty())
    {



        if(filename.find(".")!=std::string::npos)
        {
            //has extension, checking that the extension is .lua

            if(filename.substr(filename.find_last_of(".") + 1) != "lua")
            {
                QErrorMessage* error_message = new QErrorMessage();

                error_message->showMessage("Invalid Extension");
                return;
            }
        }
        else
        {
            filename +=".lua";

        }

        filename = "Action"+filename;

        //check if the file exists already


        std::ifstream file(filename.c_str());

        if(file.is_open())
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Warning", "The file exists. Do you want overwrite it?",
                                          QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::No)
            {
                file.close();
                return;
            }
        }

        std::cout << "Creating file " << filename <<std::endl;

        std::ofstream outfile (filename);
        outfile << "--script created with BT GUI" << std::endl;
        outfile.close();

        std::unique_ptr<NodeDataModel> dataModel = _main_scene->registry().create("Action");

        BehaviorTreeNodeModel& node_on_scene = (BehaviorTreeNodeModel&)_main_scene->createNode( std::move(dataModel) );

        node_on_scene.lastComboItem();
        NodeReorder(*_main_scene);


    }
}

void MainWindow::on_actionAdd_Condition_triggered()
{
    bool ok;
    std::string filename;
    QString text = QInputDialog::getText(0, "New Condition",
                                         "filename:", QLineEdit::Normal,
                                         "", &ok);

    filename = text.toStdString();
    if (ok && !text.isEmpty())
    {



        if(filename.find(".")!=std::string::npos)
        {
            //has extension, checking that the extension is .lua

            if(filename.substr(filename.find_last_of(".") + 1) != "lua")
            {
                QErrorMessage* error_message = new QErrorMessage();

                error_message->showMessage("Invalid Extension");
                return;
            }
        }
        else
        {
            filename +=".lua";

        }

        filename = "Condition"+filename;

        //check if the file exists already


        std::ifstream file(filename.c_str());

        if(file.is_open())
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Warning", "The file exists. Do you want overwrite it?",
                                          QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::No)
            {
                file.close();
                return;
            }
        }

        std::cout << "Creating file " << filename <<std::endl;

        std::ofstream outfile (filename);
        outfile << "--script created with BT GUI" << std::endl;
        outfile.close();

        std::unique_ptr<NodeDataModel> dataModel = _main_scene->registry().create("Condition");

        BehaviorTreeNodeModel& node_on_scene = (BehaviorTreeNodeModel&)_main_scene->createNode( std::move(dataModel) );

        node_on_scene.lastComboItem();
        NodeReorder(*_main_scene);

    }
}


void MainWindow::onTimerUpdate()
{
    calculateForces(_main_scene);
}


void MainWindow::on_selectMode_sliderReleased()
{
//    if(!is_BT_valid(_main_scene))
//    {
//        int ret = QMessageBox::warning(this, tr("Oops!"),
//                                       tr("Invalid behavior tree. There must be a root node"),
//                                       QMessageBox::Ok);
//        return;
//    }

//    const int new_value = (ui->selectMode->value() == 0) ? 1 : 0;
//    ui->selectMode->setValue( new_value );
}

void MainWindow::on_playButton_released()
{

    qDebug() << "on_playButton_released\n";
        if(!is_BT_valid(_main_scene))
        {
            int ret = QMessageBox::warning(this, tr("Oops!"),
                                           tr("Invalid behavior tree. There must be a root node"),
                                           QMessageBox::Ok);
            ui->playButton->setChecked(false);
            return;
        }

    bool locked = ui->playButton->isChecked();
    qDebug() << "Locking\n";

    //lockEditing( locked );
    qDebug() << "Locked\n";

    if (locked)
    {

        setMode(1);

        for (auto& it: _main_scene->nodes() )
        {
            QtNodes::Node* node = it.second.get();
            node->nodeGraphicsObject().update();
        }
        createPacmanXml();
        std::thread t(&runTree, _main_scene);
        t.detach();

    }
    else
    {
        setMode(0);
    }



}

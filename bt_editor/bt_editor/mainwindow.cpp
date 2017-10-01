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
#include "ActionNodeModel.hpp"
#include "ConditionNodeModel.hpp"
#include "DecoratorNodeModel.hpp"
//#include "SubtreeNodeModel.hpp"

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

  //ret->registerModel<IfThenElseModel>("Control");
  ret->registerModel<EscapeNodeModel>("Action");
  ret->registerModel<GreedyNodeModel>("Action");
  ret->registerModel<IsCloseConditionNodeModel>("Condition");
  ret->registerModel<DecoratorNodeModel>("Decorator");
  //ret->registerModel<SubtreeNodeModel>("SubTree");

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

#ifdef USING_ROS
  _state_subscriber = _node_handle.subscribe("/behavior_tree/current_state", 10, &MainWindow::callbackState, this);
#endif
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
  NodeFactory::loadMetaModelFromXML(domDocument);

  _main_scene->clearScene();
  QtNodes::Node& first_qt_node = _main_scene->createNode( _main_scene->registry().create("Root"));

  std::cout<<	"Starting parsing"<< std::endl;
  ParseBehaviorTreeXML(behaviortree_root, _main_scene, first_qt_node);
  std::cout<<"XML Parsed Successfully!"<< std::endl;

  NodeReorder( *_main_scene );

  for (auto& it: _main_scene->nodes() )
  {
    QtNodes::Node* node = it.second.get();

    ActionNodeModel* action_model = dynamic_cast<ActionNodeModel*>( node->nodeDataModel() );
    if( action_model )
    {
      connect( action_model, &ActionNodeModel::adjustSize, this, &MainWindow::onNodeSizeChanged);
    }
  }

  lockEditing( ui->selectMode->value() == 1 );
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
  if( model_name == "Action" || model_name == "Decorator")
  {
    const BehaviorTreeNodeModel* action_node = dynamic_cast<const BehaviorTreeNodeModel*>(node_model);
    if( action_node )
    {
      element.setAttribute("ID", action_node->type() );
    }
    auto parameters = action_node->getCurrentParameters();
    for(const auto& param: parameters)
    {
      element.setAttribute( param.first, param.second );
    }
  }

  if( element.attribute("ID") != node_model->caption())
  {
    element.setAttribute("name", node_model->caption() );
  }
  parent_element.appendChild( element );


  auto node_children = getChildren(*_main_scene, *node );
  for(QtNodes::Node* child : node_children)
  {
    recursivelyCreateXml(doc, element, child );
  }
}


void MainWindow::on_actionSave_triggered()
{
  std::vector<QtNodes::Node*> roots = findRoots( *_main_scene );
  bool valid_root = (roots.size() == 1) && ( dynamic_cast<RootNodeModel*>(roots.front()->nodeDataModel() ));

  QtNodes::Node* current_node = nullptr;

  if( valid_root ){
    auto root_children = getChildren(*_main_scene, *roots.front() );
    if( root_children.size() == 1){
      current_node = root_children.front();
    }
    else{
      valid_root = false;
    }
  }

  if( !valid_root || !current_node)
  {
    int ret = QMessageBox::warning(this, tr("Oops!"),
                                   tr("Malformed behavior tree. There must be only 1 root node"),
                                   QMessageBox::Ok);
    return;
  }

  //----------------------------
  QDomDocument doc;
  QDomElement root_element = doc.createElement( "root" );
  doc.appendChild( root_element );

  QDomElement bt_node = doc.createElement( "BehaviorTree" );
  root_element.appendChild(bt_node);


  recursivelyCreateXml(doc, bt_node, current_node );

  //-------------------------------------
  QSettings settings("Michele Colledanchise", "BehaviorTreeEditor");
  QString directory_path  = settings.value("MainWindow.lastSaveDirectory",
                                           QDir::currentPath() ).toString();

  QFileDialog saveDialog;
  saveDialog.setAcceptMode(QFileDialog::AcceptSave);
  saveDialog.setDefaultSuffix("xml");
  saveDialog.setNameFilter("State Machine (*.xml)");
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
  QSettings settings("EurecatRobotics", "BehaviorTreeEditor");
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
  const int new_value = (ui->selectMode->value() == 0) ? 1 : 0;
  ui->selectMode->setValue( new_value );
}

void MainWindow::on_selectMode_valueChanged(int value)
{

  bool locked = value == 1;
  lockEditing( locked );

  QFont fontA = ui->labelEdit->font();
  fontA.setBold( !locked );
  ui->labelEdit->setFont( fontA );

  QFont fontB = ui->labelMonitor->font();
  fontB.setBold( locked );
  ui->labelMonitor->setFont( fontB );

  if (locked)
  {
      std::cout << "RUNNING" << std::endl;
      setMode(1);

      for (auto& it: _main_scene->nodes() )
      {
        QtNodes::Node* node = it.second.get();
       // node->nodeGraphicsObject().update(); // to color ti
      }

      runTree(_main_scene);
      //std::thread t(&runTree, _main_scene);
      //t.detach();

  }
  else
  {
      std::cout << "STOP" << std::endl;

      setMode(0);
  }
}




void MainWindow::on_actionAdd_Action_triggered()
{
    bool ok;
    std::string filename;
    // Ask for birth date as a string.
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
            if (reply == QMessageBox::Yes)
            {
//                file << "--script created with BT GUI" << std::endl;
//                file.close();
//                return;
            }
            else
            {
                return;
            }
        }

        std::cout << "Creating file " << filename <<std::endl;

        std::ofstream outfile (filename);
        outfile << "--script created with BT GUI" << std::endl;
        outfile.close();

        std::unique_ptr<NodeDataModel> dataModel = _main_scene->registry().create("Action");

        BehaviorTreeNodeModel& node_on_scene = (BehaviorTreeNodeModel&)_main_scene->createNode( std::move(dataModel) );


    }
}

void MainWindow::on_actionAdd_Condition_triggered()
{

}


void MainWindow::onTimerUpdate()
{
  calculateForces(_main_scene);
}







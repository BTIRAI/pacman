#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <node_editor/Node>
#include <QDomDocument>
#include <QShortcut>
#include <QTimer>
#include <deque>
#include <thread>
#include <mutex>

#ifdef USING_ROS
#include <ros/ros.h>
#include <std_msgs/String.h>
#endif
namespace Ui {
class MainWindow;
}

namespace QtNodes{
class FlowView;
class FlowScene;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_actionLoad_triggered();

    void on_actionSave_triggered();

    void on_actionZoom_In_triggered();

    void on_actionZoom_ut_triggered();

    void on_actionAuto_arrange_triggered();

    void onNodeMoved();

    void onNodeSizeChanged();

    void onSceneChanged();

    virtual void closeEvent(QCloseEvent *event) override;

    void on_selectMode_sliderPressed();

    void on_selectMode_valueChanged(int value);

    void onTimerUpdate();


#ifdef USING_ROS
    void on_actionLoadRosparam_triggered();
#endif

    void on_actionAdd_Action_triggered();

    void on_actionAdd_Condition_triggered();

signals:
    void updateGraphic();

private:
    Ui::MainWindow *ui;

    void lockEditing(const bool locked);

    void updateStates(QXmlInputSource* source);

    QtNodes::FlowScene* _main_scene;
    QtNodes::FlowView*  _main_view;

    bool _node_moved;

    QShortcut _arrange_shortcut;

    void recursivelyCreateXml(QDomDocument& doc, QDomElement& parent_element, const QtNodes::Node* node);

    QTimer _periodic_timer;

    std::mutex _mutex;

    void loadFromXML(const QString &xml_text);

    bool    _state_received;
    QString _state_msg;
    QtNodes::Node* _root_node;


#ifdef USING_ROS
    ros::Subscriber _state_subscriber;
    ros::NodeHandle _node_handle;
    void callbackState(const std_msgs::String::ConstPtr& msg);
#endif
};

#endif // MAINWINDOW_H

#ifndef NODE_UTILS_H
#define NODE_UTILS_H

#include <node_editor/NodeData>
#include <node_editor/FlowScene>


std::vector<QtNodes::Node*> findRoots(const QtNodes::FlowScene &scene);

std::vector<QtNodes::Node*> getChildren(const QtNodes::FlowScene &scene,
                                        const QtNodes::Node &parent_node);




void NodeReorder(QtNodes::FlowScene& scene);

void SubtreeReorder(QtNodes::FlowScene &scene, QtNodes::Node &root_node);

void runTree(QtNodes::FlowScene *scene);

void calculateForces(QtNodes::FlowScene *scene);

void setMode(int mode);

int getMode();

bool is_BT_valid(QtNodes::FlowScene* scene);


#endif // NODE_UTILS_H

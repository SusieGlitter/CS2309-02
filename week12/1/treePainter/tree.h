#ifndef TREE_H
#define TREE_H

#include <QWidget>
#include <vector>
#include <map>
#include <QDebug>

#include "node.h"

class Tree
{
public:
    Tree();
    ~Tree();
    void clear(Node *now);
    bool updateTree(QString s);
    void drawTree(QPainter &painter,Node *now);
    int r;
    int canvash;
    int canvasw;
    double hDis;
    double wDis;
    double xmax,xmin;
    double ymax,ymin;
    Node *root;
    std::vector<Node> nodes;
    std::vector<std::vector<Node *>>layers;
    std::map<QString,int> mapQstrInt;
    int mapcnt;
    void adjust1(Node *now);//调整至对称
    bool adjust2_1(Node *now);//调整至不重叠
    void adjust2_2(Node *node1,Node *node2);
    void adjust3_1(Node *now);//调整至居中
    void adjust3_2(Node *now,double deltaX,double deltaY);
};


#endif // TREE_H

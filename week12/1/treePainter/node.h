#ifndef NODE_H
#define NODE_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <vector>

class Node
{
public:
    QString name;
    double x;
    double y;
    int depth;
    Node();
    Node(const Node &that);
    Node(QString name);
    ~Node();
    void drawNode(QPainter &painter,int r);
    std::vector<Node*> sons;
    Node* fat;
    Node* neighbor;
};


#endif // NODE_H

#include "node.h"

Node::Node()
{
    this->name=QString();
    this->x=0;
    this->y=0;
    this->fat=nullptr;
}

Node::Node(const Node &that)
{
    this->name=that.name;
    this->x=that.x;
    this->y=that.y;
    this->fat=nullptr;
}

Node::Node(QString name)
{
    this->name=name;
    this->fat=nullptr;
}

Node::~Node()
{

}

void Node::drawNode(QPainter &painter,int r)
{
    QPainterPath path;
    path.addEllipse(x-r,y-r,2*r,2*r);
    path.setFillRule(Qt::OddEvenFill);
    painter.drawPath(path);
    // painter.drawEllipse(QPoint(x,y),r,r);
    painter.drawText(QRectF(x-4*r,y-r,8*r,2*r),Qt::AlignHCenter|Qt::AlignVCenter,name);
}

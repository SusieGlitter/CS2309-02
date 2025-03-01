#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    ui->label->installEventFilter(this);

    tree=new Tree;
    tree->r=ui->horizontalSlider->value();
    tree->hDis=ui->horizontalSlider_2->value();
    tree->wDis=ui->horizontalSlider_3->value();
    tree->canvasw=560;
    tree->canvash=560;
    tree->updateTree(ui->plainTextEdit->toPlainText());
    update();
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::eventFilter(QObject *obj, QEvent *event)
{
    if(obj==ui->label&&event->type()==QEvent::Paint)
        labelPaint();
    return QWidget::eventFilter(obj,event);
}


void Widget::on_pushButton_clicked()
{
    update();
}

void Widget::labelPaint()
{
    QPainter painter(ui->label);
    QPen pen;
    pen.setWidth(3);
    pen.setColor(QColor(0,0,0));
    painter.setPen(pen);
    painter.setBrush(QColor(0,255,0));
    painter.setFont(QFont("Arial",tree->r));
    tree->drawTree(painter,tree->root);
}





void Widget::on_plainTextEdit_textChanged()
{
    tree->updateTree(ui->plainTextEdit->toPlainText());
    update();
}

void Widget::on_horizontalSlider_valueChanged(int value)
{
    tree->r=value;
    tree->updateTree(ui->plainTextEdit->toPlainText());
    update();
}



void Widget::on_horizontalSlider_2_valueChanged(int value)
{
    tree->hDis=value;
    tree->updateTree(ui->plainTextEdit->toPlainText());
    update();
}


void Widget::on_horizontalSlider_3_valueChanged(int value)
{
    tree->wDis=value;
    tree->updateTree(ui->plainTextEdit->toPlainText());
    update();
}


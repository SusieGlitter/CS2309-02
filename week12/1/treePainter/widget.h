#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

#include "node.h"
#include "tree.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    bool eventFilter(QObject *obj,QEvent *event);
    void on_pushButton_clicked();
    void labelPaint();


    void on_horizontalSlider_valueChanged(int value);


    void on_plainTextEdit_textChanged();

    void on_horizontalSlider_2_valueChanged(int value);

    void on_horizontalSlider_3_valueChanged(int value);

private:
    Ui::Widget *ui;
    Tree *tree;
};
#endif // WIDGET_H

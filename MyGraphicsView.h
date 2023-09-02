#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QtWidgets>
#include "mainwindow.h"

class MainWindow;

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT

private :
    MainWindow *parent;

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

public:
    MyGraphicsView(MainWindow *window);
};



#endif

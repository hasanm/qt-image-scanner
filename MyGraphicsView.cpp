#include <QtWidgets>
#include "MyGraphicsView.h"
#include "mainwindow.h"

MyGraphicsView::MyGraphicsView(MainWindow *window)
{
    parent = window;
}

void MyGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    // qDebug() << "Double Clicked " << event->pos();
}

void MyGraphicsView::mousePressEvent(QMouseEvent* event)
{
    // qDebug() << "GV Mouse Pressed " << event->pos();
    parent->onMousePressed(event);
}


void MyGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    // qDebug() << "GV Mouse Released " << event->pos();
    parent->onMouseReleased(event);
}


void MyGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    // qDebug() << "GV Moved " << event->pos();
    parent->onMouseMoved(event);
}
